#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "SpriteComponent.h"
#include "Collider.h"
#include <unordered_set>

// this class works well with the made game but is not ready for general purpose use

namespace engine {
    class Rigidbody : public SpriteComponent
    {
    public:
        Rigidbody(Sprite* sprite) : SpriteComponent(sprite) {}

        void update(std::vector<GameObject*> objs) override; 

        /// <summary> Om gravitationen önskas vara större eller mindre. </summary>
        /// <param name="mult"> multiplicerar med standardvärdet. 0 för att inte ha nån gravitation. </param>
        void setGravityMultiplier(const float mult) { gravityMultiplier = mult; }
        float getGravityMultiplier() const { return gravityMultiplier; }

        /// <summary> Sätter hur mycket som figuren ska "studsa" vid kollision. </summary>
        /// <param name="mult"> faktorn som multipliceras in vid kollision. Lägre = mindre "studs". 0 för att inte ha nån "studs". </param>
        void setElasticityMultiplier(const float mult) { elasticityMultiplier = mult; }
        float getElasticityMultiplier() const { return elasticityMultiplier; }

        /// <summary> Lägger på en kraft på figuren. OBS att funktionen ser till att det rör sig lika fort oavsett FPS m.h.a Session::deltaTime. </summary>
        /// <param name="vec"> kraften som ska appliceras i form av en Vector(x, y). </param>
        void addForce(const Vector force) { velocity += force; }
        void addForceX(const float x) { velocity.x += x; }
        void addForceY(const float y) { velocity.y += y; }

        /// <summary> Sätter hastigheten som figuren ska röra sig. OBS att funktionen lägger på Session::deltaTime så det rör sig lika oavsett FPS. </summary>
        /// <param name="vel"> hastigheten i form av en vektor. (finns överlagrad med 2 floats) </param>
        void setVelocity(const Vector vel) { velocity = vel; }
        void setVelocity(const float velX, const float velY) { setVelocity(Vector(velX, velY)); }
        void setVelocityY(const float y) { velocity.y = y; }
        void setVelocityX(const float x) { velocity.x = x; }

        Vector getVelocity() const { return velocity; }

        bool isGrounded() const; 

        // Om den är trigger händer inget vid kollision 
        void setIsTrigger(const bool toggle) { isTrigger = toggle; }

        // Om figuren ska sakta ned i x-led när den rör sig, aktivt som standard
        void enableFriction(const bool toggle) { frictionIsActive = toggle; }
        bool hasFriction() const { return frictionIsActive; }

    private:
        enum class CollisionSide {
            NONE = -1, TOP, RIGHT, BOTTOM, LEFT
        };

        bool isTrigger = false; // Om den är trigger ska inget hända vid kollision 
        bool frictionIsActive = true;

        float gravityMultiplier = 1.0f; 
        Vector velocity = Vector(0, 0); 
        float elasticityMultiplier = 0.5f; 

        void updateGravity(); // Öker farten nedåt i y-axeln (eller saktar ned en rörelse uppåt) om figuren inte står på nåt eller gravitationen är avstängd
        void decreaseVelocity(); // Sänker hastigheten i x-led (friktion)

        void collided(GameObject* other); // Funktionen som kallas från Collider vid kollision och sedan utför aktionen som behövs, t.ex vänder velY vid kollision med marken
        CollisionSide getCollisionSide(const GameObject* other) const;
        void move(std::vector<GameObject*>& objs);
        void checkIfMoveIsPossible(Vector& newPos, Vector& oldPos, Collider* coll);

        friend class Collider; 
    };
}

#endif
