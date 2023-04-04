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

        /// <summary> Används för att aktivera pixel-kollisioner. OBS att kollisioner endast sker där båda objekten har (nån typ av) kollisioner aktiverat. 
        /// För att stänga av kollisioner helt: Använd setEnabled(false) på collider komponenten. </summary>
        /// <param name="toggle"> true/false om pixel-kollisioner ska vara på/av. Om av så används rect-kolls.  </param>
        void togglePixelCollisions(bool toggle) { pixelPerfectColls = toggle; }

        bool isPixelCollisionsActive() const { return pixelPerfectColls; }

    private: 
        std::unordered_set<GameObject*> collidingObjects;
        bool pixelPerfectColls = false;

        bool checkCollision(GameObject* other);
        bool checkPixelCollision(GameObject* other, SDL_Rect& rect, SDL_Rect& otherRect);
        void checkCollisions(const std::vector<GameObject*>& objects); 
        bool collisionIsPossible(GameObject* other) const;

        friend class Rigidbody; // Rb behöver tillgång till kollisioner 
    };
}

#endif