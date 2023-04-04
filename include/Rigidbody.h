#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "SpriteComponent.h"
#include "Collider.h"
#include <unordered_set>

// OBS DENNA KLASS HAR FORTFARANDE EN DEL BUGGAR. FR�MST G�LLANDE ELASTICITET OCH N�R DEN ANV�NDS TSM MED PIXELKOLLISIONER.

namespace engine {
    class Rigidbody : public SpriteComponent
    {
    public:
        Rigidbody(Sprite* sprite) : SpriteComponent(sprite) {}

        void update(std::vector<GameObject*> objs) override; 

        /// <summary> Om gravitationen �nskas vara st�rre eller mindre. </summary>
        /// <param name="mult"> multiplicerar med standardv�rdet. 0 f�r att inte ha n�n gravitation. </param>
        void setGravityMultiplier(const float mult) { gravityMultiplier = mult; }
        float getGravityMultiplier() const { return gravityMultiplier; }

        /// <summary> S�tter hur mycket som figuren ska "studsa" vid kollision. </summary>
        /// <param name="mult"> faktorn som multipliceras in vid kollision. L�gre = mindre "studs". 0 f�r att inte ha n�n "studs". </param>
        void setElasticityMultiplier(const float mult) { elasticityMultiplier = mult; }
        float getElasticityMultiplier() const { return elasticityMultiplier; }

        /// <summary> L�gger p� en kraft p� figuren. OBS att funktionen ser till att det r�r sig lika fort oavsett FPS m.h.a Session::deltaTime. </summary>
        /// <param name="vec"> kraften som ska appliceras i form av en Vector(x, y). </param>
        void addForce(const Vector force) { velocity += force; }
        void addForceX(const float x) { velocity.x += x; }
        void addForceY(const float y) { velocity.y += y; }

        /// <summary> S�tter hastigheten som figuren ska r�ra sig. OBS att funktionen l�gger p� Session::deltaTime s� det r�r sig lika oavsett FPS. </summary>
        /// <param name="vel"> hastigheten i form av en vektor. (finns �verlagrad med 2 floats) </param>
        void setVelocity(const Vector vel) { velocity = vel; }
        void setVelocity(const float velX, const float velY) { setVelocity(Vector(velX, velY)); }
        void setVelocityY(const float y) { velocity.y = y; }
        void setVelocityX(const float x) { velocity.x = x; }

        Vector getVelocity() const { return velocity; }

        bool isGrounded() const; 

        // Om den �r trigger h�nder inget vid kollision 
        void setIsTrigger(const bool toggle) { isTrigger = toggle; }

        // Om figuren ska sakta ned i x-led n�r den r�r sig, aktivt som standard
        void enableFriction(const bool toggle) { frictionIsActive = toggle; }
        bool hasFriction() const { return frictionIsActive; }

    private:
        enum class CollisionSide {
            NONE = -1, TOP, RIGHT, BOTTOM, LEFT
        };

        bool isTrigger = false; // Om den �r trigger ska inget h�nda vid kollision 
        bool frictionIsActive = true;

        float gravityMultiplier = 1.0f; 
        Vector velocity = Vector(0, 0); 
        float elasticityMultiplier = 0.5f; 

        void updateGravity(); // �ker farten ned�t i y-axeln (eller saktar ned en r�relse upp�t) om figuren inte st�r p� n�t eller gravitationen �r avst�ngd
        void decreaseVelocity(); // S�nker hastigheten i x-led (friktion)

        void collided(GameObject* other); // Funktionen som kallas fr�n Collider vid kollision och sedan utf�r aktionen som beh�vs, t.ex v�nder velY vid kollision med marken
        CollisionSide getCollisionSide(const GameObject* other) const;
        void move(std::vector<GameObject*>& objs);
        void checkIfMoveIsPossible(Vector& newPos, Vector& oldPos, Collider* coll);

        friend class Collider; 
    };
}

#endif