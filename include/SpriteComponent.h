#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <vector>

#include "GameObject.h"

namespace engine {

	class Sprite; // Forward declaration 

	class SpriteComponent 
	{
	public:
		virtual void update(std::vector<GameObject*> objs) = 0; // Denna gör det som ska ske när den kallas (förmodligen varje frame) T.ex kollar Collider kollisioner

		bool isEnabled() const { return enabled; }
		void setEnabled(const bool toggle) { enabled = toggle; }

		virtual ~SpriteComponent() {}

	protected:
		SpriteComponent(Sprite* sprite) : sprite(sprite) {}

		Sprite* getSprite() const { return sprite; }

	private:
		Sprite* sprite; // Spriten som komponenten är kopplad till 
		bool enabled = true; 
	};
}

#endif