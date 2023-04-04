#include "GameObject.h"
#include "System.h"
#include "Scene.h"
#include "Text.h"
#include "TestScene.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <vector>

namespace engine {

	GameObject::GameObject(int x, int y) : rect({x, y, 0, 0}), actualPosX(x), actualPosY(y) // 0 är placeholders och ändras i subklassernas konstruktorer
	{
		name = "GameObject" + std::to_string(objectCount);
		objectCount++;
	}

	void GameObject::setX(float x)
	{
		if (stopAtWindowBorder) {
			if (x < 0)
				return;
			int w;
			SDL_GetWindowSize(sys.getWindow(), &w, NULL);
			if (x + getW() > w)
				return;
		}

		float posDiff = x - getX();
		actualPosX = x; 
		rect.x = x;
		for (auto& c : children) 
			c->setX(c->getX() + posDiff);
	}

	void GameObject::setY(float y)
	{
		if (stopAtWindowBorder) {
			if (y < 0)
				return;
			int h;
			SDL_GetWindowSize(sys.getWindow(), NULL, &h);
			if (y + getH() > h)
				return;
		}

		float posDiff = y - getY();
		actualPosY = y;
		rect.y = y;
		for (auto& c : children)
			c->setY(c->getY() + posDiff);
		
	}

	void GameObject::setW(int w)
	{
		int difference = w - getW();
		rect.w = w;
		for (auto& c : children) {
			c->setW(c->getW() + difference);
			if (Text* text = dynamic_cast<Text*>(c)) 
				text->setMaxWidth(getW()); 
		}
	}

	void GameObject::setH(int h)
	{
		int difference = h - getH();
		rect.h = h;
		for (auto& c : children) {
			c->setH(c->getH() + difference);
			if (Text* text = dynamic_cast<Text*>(c))
				text->setMaxWidth(text->getMaxWidth()); 
		}
	}

	void GameObject::addChild(GameObject* obj)
	{
		children.push_back(obj);
		obj->parent = this; 
	}

	GameObject* GameObject::removeChild(GameObject* obj)
	{
		auto it = children.begin();

		while (it != children.end()) {

			if ((*it) == obj) {
				it = children.erase(it);
				obj->parent = nullptr; 
				return obj; 
			}
			else ++it;
		}
		return nullptr; 
	}

	void GameObject::setActive(const bool toggle)
	{
		active = toggle; 
		for (auto& c : children)
			c->active = toggle; 
	}

	void GameObject::update()
	{
		if(updateMethodToCall != nullptr)
			updateMethodToCall(); 
	}

	void GameObject::setRotation(int rotation) {
		rotationAngle = rotation; 
		for (auto& c : children)
			c->setRotation(rotation); 
	}

	void GameObject::flipX() {  
		if (flipType == SDL_FLIP_HORIZONTAL) // Redan flippad endast horisontellt 
			flipType = SDL_FLIP_NONE;
		else if (flipType == 3) // Flippad både x och y 
			flipType = SDL_FLIP_VERTICAL; 
		else // Inte flippad alls eller flippad endast vertikalt  
			flipType = (SDL_RendererFlip)(flipType | SDL_FLIP_HORIZONTAL);
		for (auto& c : children)
			c->flipX(); 
	}

	void GameObject::flipY() {
		if (flipType == SDL_FLIP_VERTICAL) // Redan flippad endast vertikalt  
			flipType = SDL_FLIP_NONE;
		else if (flipType == 3) // Flippad både x och y
			flipType = SDL_FLIP_HORIZONTAL;
		else // Inte flippad alls eller flippad endast horisontellt 
			flipType = (SDL_RendererFlip)(flipType | SDL_FLIP_VERTICAL);
		for (auto& c : children)
			c->flipY(); 
	}

	void GameObject::resetFlip() {
		flipType = SDL_FLIP_NONE;
		for (auto& c : children)
			c->resetFlip(); 
	}

	GameObject::~GameObject() {
		objectCount--; 
	}
}