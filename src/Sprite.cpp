#include "Sprite.h"
#include "Constants.h"
#include "System.h"
#include "SpriteComponent.h"

#include <SDL2/SDL_image.h>
#include <string>
#include <iostream> // Felmeddelanden 
#include <algorithm> // clamp 
#include "Collider.h"

namespace engine {

	Sprite::Sprite(int x, int y, std::string fileName) : GameObject(x, y)
	{
		surface = IMG_Load((constants::gResPath + "images/" + fileName).c_str());
		if (surface == NULL) {
			std::cout << "Kunde inte ladda in " << fileName << "\nFelmeddelande: " << IMG_GetError() << std::endl;
			return;
		}
		//texture = IMG_LoadTexture(sys.getRen(), (constants::gResPath + "/images/" + fileName).c_str());
		texture = SDL_CreateTextureFromSurface(sys.getRen(), surface); 
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		setW(w);
		setH(h); 
	}

	Sprite* Sprite::getInstance(int x, int y, std::string fileName)
	{
		return new Sprite(x, y, fileName);
	}

	void Sprite::draw() const
	{
		const SDL_Rect& rect = getRect();
		SDL_RenderCopyEx(sys.getRen(), texture, NULL, &rect, getRotation(), NULL, getFlipType());
	}

	void Sprite::changeImage(std::string fileName, bool keepSize)
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		surface = IMG_Load((constants::gResPath + "/images/" + fileName).c_str());
		if (surface == NULL) {
			std::cout << "Kunde inte ladda in " << fileName << "\nFelmeddelande: " << IMG_GetError() << std::endl;
			return;
		}
		texture = SDL_CreateTextureFromSurface(sys.getRen(), surface);
		if(!keepSize) {
			int w, h;
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			setW(w);
			setH(h);
		}
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(texture, color.a); 
	}

	void Sprite::setColor(int r, int g, int b, int a)
	{
		r = std::clamp(r, 0, 255);
		g = std::clamp(g, 0, 255);
		b = std::clamp(b, 0, 255);
		a = std::clamp(a, 0, 255);
		color.r = r;
		color.g = g; 
		color.b = b; 
		color.a = a; 
		SDL_SetTextureColorMod(texture, r, g, b); 
		SDL_SetTextureAlphaMod(texture, a); 
	}

	void Sprite::setColor(SDL_Color& color)
	{
		setColor(color.r, color.g, color.b, color.a); 
	}

	void Sprite::setColorAlpha(int a)
	{
		color.a = a; 
		setColor(color); 
	}

	void Sprite::engineUpdate(std::vector<GameObject*> objs)
	{
		for (auto c : components)
			c->update(objs);
	}

	Sprite::~Sprite()
	{
		SDL_DestroyTexture(texture); 
		SDL_FreeSurface(surface); 
		for (auto c : components) // Just nu har de inget att ta bort men kan vara bra att kalla ifall de får i framtiden 
			delete c; 
	}
}
