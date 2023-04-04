#ifndef COLLIDER_H
#define COLLIDER_H

#include <unordered_set>

#include "SpriteComponent.h"

namespace engine {
    class Collider : public SpriteComponent
    {
    public:
        Collider(Sprite* sprite) : SpriteComponent(sprite) {}

        void update(std::vector<GameObject*> objs) override;  

        /// <summary> Anv�nds f�r att aktivera pixel-kollisioner. OBS att kollisioner endast sker d�r b�da objekten har (n�n typ av) kollisioner aktiverat. 
        /// F�r att st�nga av kollisioner helt: Anv�nd setEnabled(false) p� collider komponenten. </summary>
        /// <param name="toggle"> true/false om pixel-kollisioner ska vara p�/av. Om av s� anv�nds rect-kolls.  </param>
        void togglePixelCollisions(bool toggle) { pixelPerfectColls = toggle; }

        bool isPixelCollisionsActive() const { return pixelPerfectColls; }

    private: 
        std::unordered_set<GameObject*> collidingObjects;
        bool pixelPerfectColls = false;

        bool checkCollision(GameObject* other);
        bool checkPixelCollision(GameObject* other, SDL_Rect& rect, SDL_Rect& otherRect);
        void checkCollisions(const std::vector<GameObject*>& objects); 
        bool collisionIsPossible(GameObject* other) const;

        friend class Rigidbody; // Rb beh�ver tillg�ng till kollisioner 
    };
}

#endif