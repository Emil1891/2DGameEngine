#include "Collider.h"

#include <SDL2/SDL.h>
#include <algorithm>

#include "Sprite.h"
#include "Rigidbody.h"

// Hj�lp funktioner 
static bool isPixelTransparent(Uint32 pixel, SDL_PixelFormat* format) { // surface->format
	Uint8 r, g, b, a;
	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
	return a == 0; // 0 = helt transparent 
}

static Uint32 getPixel(SDL_Surface* surf, int x, int y) { // x, y �r relativt till surfacen 
	if (x > surf->w || y > surf->h) // Om out of bounds
		return -1;
	SDL_LockSurface(surf);
	Uint32 pixel = 0;
	int bpp = surf->format->BytesPerPixel;
	char* p = (char*)surf->pixels + y * surf->pitch + x * bpp;
	switch (bpp) {
	case 1:
		pixel = *p;
		break;
	case 2:
		pixel = *(Uint16*)p;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			pixel = p[0] << 16 | p[1] << 8 | p[2];
		else
			pixel = p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		pixel = *(Uint32*)p;
		break;
	}
	SDL_UnlockSurface(surf);
	return pixel;
}

namespace engine {

	void Collider::update(std::vector<GameObject*> objs)
	{
		if (isEnabled()) 
			checkCollisions(objs);
		else
			collidingObjects.clear(); // S� det �terst�lls om man avaktiverar collidern 
	}

	bool Collider::checkCollision(GameObject* other) 
	{
		SDL_Rect rect = getSprite()->getRect();
		SDL_Rect otherRect = other->getRect();
		if (!pixelPerfectColls) 
			return SDL_HasIntersection(&rect, &otherRect);
		else 
			return checkPixelCollision(other, rect, otherRect); 
	}

	bool Collider::checkPixelCollision(GameObject* other, SDL_Rect& rect, SDL_Rect& otherRect)
	{
		SDL_Rect interRect;
		if (SDL_IntersectRect(&rect, &otherRect, &interRect)) { // Om rect kollision 
			if (Sprite* otherSprite = dynamic_cast<Sprite*>(other)) {
				for (int row = 0; row < interRect.w; row++) { // F�r varje rad i interRect 
					for (int col = 0; col < interRect.h; col++) { // G� igenom alla kolumner 
						// * Se kommentar l�ngre ned f�r vad som h�nder p� raderna nedan 
						int surfRelX = interRect.x - getSprite()->getX() + row;
						int surfRelY = interRect.y - getSprite()->getY() + col;
						int otherSurfRelX = interRect.x - otherSprite->getX() + row;
						int otherSurfRelY = interRect.y - otherSprite->getY() + col;
						Uint32 pixel = getPixel(getSprite()->surface, surfRelX, surfRelY);
						Uint32 pixelOther = getPixel(otherSprite->surface, otherSurfRelX, otherSurfRelY);
						if (pixel == -1 || pixelOther == -1) // Om interRectens koordinat ligger utanf�r n�gons bild returneras -1
							continue;
						else if (!isPixelTransparent(pixel, getSprite()->surface->format) && !isPixelTransparent(pixelOther, otherSprite->surface->format)) // B�das bild (pixel) m�ste vara icke transparent 
							return true;
					}
				}
			}
		}
		return false; // Om ingen rect kollision
	}
	// *^ Pixeln h�mtas fr�n en koordinat p� surfacen d�r 0, 0 �r surfacens �vre v�nsta h�rn (allts� ej relativt till f�nstret). Varje koordinat i interRect g�s igenom d�r man m�ste
	// omvandla interRect-pixelns koordinat till en position p� surfacen (interRect.x - surface.x + rad). Samma f�r y-koord. 

	bool Collider::collisionIsPossible(GameObject* other) const {
		if (!other->isActive())
			return false;
		if (Sprite* spr = dynamic_cast<Sprite*>(other)) { // Om sprite 
			if (spr == getSprite()) // Om sig sj�lv 
				return false;
			if (!spr->getComponent<Collider>() || !spr->getComponent<Collider>()->isEnabled()) // Kolla om andra objektet har en collider och att den �r aktiverad 
				return false;

			return true; // Passerar den alla kontroller s� �r kollision m�jlig 
		}
		else // Om inte sprite (bara sprites kan kollidera)  
			return false;
	}

	// Denna funktion kallas fr�n update. Kollar alla andra objekt om de har kollisioner aktiverat och om de kolliderar med kopplade Sprite objektet 
	void Collider::checkCollisions(const std::vector<GameObject*>& objects)
	{
		Rigidbody* rb = getSprite()->getComponent<Rigidbody>();
		std::unordered_set<GameObject*> collidedWithThisFrame;
		for (auto obj : objects) { // F�r alla objekt 
			if (collisionIsPossible(obj)) {
				if (checkCollision(obj)) {// Om kollision 
					if (!collidingObjects.contains(obj)) { // Och inte redan kolliderat 
						getSprite()->onCollisionEnter(obj);
						collidingObjects.insert(obj);
						if (rb) // Om objektet har en Rigidbody ska det ocks� ske elasticitet osv vid kollision 
							rb->collided(obj);
					}
					else // Fortsatt kollision sen tidigare 
						getSprite()->onCollisionStay(obj);
					collidedWithThisFrame.insert(obj);
				}
			}
		}
		getSprite()->colliding = !collidedWithThisFrame.empty();

		// Efter alla objekt kollats
		auto it = collidingObjects.begin();
		while (it != collidingObjects.end()) { // F�r varje objekt som har kolliderat 
			if (!collidedWithThisFrame.contains((*it))) { // Men inte g�r det l�ngre 
				try {
					getSprite()->onCollisionExit((*it));
				}
				catch (std::bad_alloc) { // Ifall man t.ex tar bort objektet n�r man kolliderar och f�rs�ker sen g�ra n�t med det i onCollExit (vilket �r dumt) s� l�ser detta att det inte krashar 
					std::cout << "Bad alloc i onCollisionExit(...), kan vara for att objektet tagits bort. \n";
				}
				it = collidingObjects.erase(it); // Tar bort objektet som inte kolliderar l�ngre 
			}
			else
				++it;
		}
		collidedWithThisFrame.clear();
	}
}