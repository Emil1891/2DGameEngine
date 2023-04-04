#include "Collider.h"

#include <SDL2/SDL.h>
#include <algorithm>

#include "Sprite.h"
#include "Rigidbody.h"

// Hjälp funktioner 
static bool isPixelTransparent(Uint32 pixel, SDL_PixelFormat* format) { // surface->format
	Uint8 r, g, b, a;
	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
	return a == 0; // 0 = helt transparent 
}

static Uint32 getPixel(SDL_Surface* surf, int x, int y) { // x, y är relativt till surfacen 
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
			collidingObjects.clear(); // Så det återställs om man avaktiverar collidern 
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
				for (int row = 0; row < interRect.w; row++) { // För varje rad i interRect 
					for (int col = 0; col < interRect.h; col++) { // Gå igenom alla kolumner 
						// * Se kommentar längre ned för vad som händer på raderna nedan 
						int surfRelX = interRect.x - getSprite()->getX() + row;
						int surfRelY = interRect.y - getSprite()->getY() + col;
						int otherSurfRelX = interRect.x - otherSprite->getX() + row;
						int otherSurfRelY = interRect.y - otherSprite->getY() + col;
						Uint32 pixel = getPixel(getSprite()->surface, surfRelX, surfRelY);
						Uint32 pixelOther = getPixel(otherSprite->surface, otherSurfRelX, otherSurfRelY);
						if (pixel == -1 || pixelOther == -1) // Om interRectens koordinat ligger utanför någons bild returneras -1
							continue;
						else if (!isPixelTransparent(pixel, getSprite()->surface->format) && !isPixelTransparent(pixelOther, otherSprite->surface->format)) // Bådas bild (pixel) måste vara icke transparent 
							return true;
					}
				}
			}
		}
		return false; // Om ingen rect kollision
	}
	// *^ Pixeln hämtas från en koordinat på surfacen där 0, 0 är surfacens övre vänsta hörn (alltså ej relativt till fönstret). Varje koordinat i interRect gås igenom där man måste
	// omvandla interRect-pixelns koordinat till en position på surfacen (interRect.x - surface.x + rad). Samma för y-koord. 

	bool Collider::collisionIsPossible(GameObject* other) const {
		if (!other->isActive())
			return false;
		if (Sprite* spr = dynamic_cast<Sprite*>(other)) { // Om sprite 
			if (spr == getSprite()) // Om sig själv 
				return false;
			if (!spr->getComponent<Collider>() || !spr->getComponent<Collider>()->isEnabled()) // Kolla om andra objektet har en collider och att den är aktiverad 
				return false;

			return true; // Passerar den alla kontroller så är kollision möjlig 
		}
		else // Om inte sprite (bara sprites kan kollidera)  
			return false;
	}

	// Denna funktion kallas från update. Kollar alla andra objekt om de har kollisioner aktiverat och om de kolliderar med kopplade Sprite objektet 
	void Collider::checkCollisions(const std::vector<GameObject*>& objects)
	{
		Rigidbody* rb = getSprite()->getComponent<Rigidbody>();
		std::unordered_set<GameObject*> collidedWithThisFrame;
		for (auto obj : objects) { // För alla objekt 
			if (collisionIsPossible(obj)) {
				if (checkCollision(obj)) {// Om kollision 
					if (!collidingObjects.contains(obj)) { // Och inte redan kolliderat 
						getSprite()->onCollisionEnter(obj);
						collidingObjects.insert(obj);
						if (rb) // Om objektet har en Rigidbody ska det också ske elasticitet osv vid kollision 
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
		while (it != collidingObjects.end()) { // För varje objekt som har kolliderat 
			if (!collidedWithThisFrame.contains((*it))) { // Men inte gör det längre 
				try {
					getSprite()->onCollisionExit((*it));
				}
				catch (std::bad_alloc) { // Ifall man t.ex tar bort objektet när man kolliderar och försöker sen göra nåt med det i onCollExit (vilket är dumt) så löser detta att det inte krashar 
					std::cout << "Bad alloc i onCollisionExit(...), kan vara for att objektet tagits bort. \n";
				}
				it = collidingObjects.erase(it); // Tar bort objektet som inte kolliderar längre 
			}
			else
				++it;
		}
		collidedWithThisFrame.clear();
	}
}