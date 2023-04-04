#include "Rigidbody.h"
#include "Collider.h"
#include "Sprite.h"

#include <cmath>
#include <unordered_map>

// OBS DENNA KLASS HAR FORTFARANDE EN DEL BUGGAR. FRÄMST GÄLLANDE ELASTICITET OCH NÄR DEN ANVÄNDS TSM MED PIXELKOLLISIONER.

namespace engine {

	void Rigidbody::update(std::vector<GameObject*> objs)
	{
		updateGravity(); 
		decreaseVelocity(); 

		move(objs); 
	}

	bool Rigidbody::isGrounded() const 
	{
		Sprite* spr = getSprite(); 
		Collider* coll = spr->getComponent<Collider>(); 
		if (!coll)
			return false;

		const int range = 30; // i pixlar (ett bra värde testades fram)
		int bottom = spr->getY() + spr->getH(); 
		for (auto c : coll->collidingObjects) { // Kollar alla objekt som kolliderar 
			int otherTop = c->getY();
			if (bottom - otherTop < range)  // Om detta objekts position längst ner är inom range av den andras top, dvs den står på det andra objektet så är den grounded
				return true;
		}
		return false;
	}

	void Rigidbody::updateGravity()
	{
		if (!isGrounded() || isTrigger)
			velocity.y += 0.001f * gravityMultiplier * Session::getDeltaTime();
		else if (velocity.y > 0 && velocity.y < 0.005f)  // Om den rör sig nedåt med liten hastighet och är grounded, förhindrar jitter som kunde ske ibland 
			velocity.y = 0;
	}

	void Rigidbody::decreaseVelocity()
	{
		if (!frictionIsActive)
			return;

		if (std::abs(velocity.x) < 0.0001f) // Stannar figuren helt om hastigheten har blivit jätteliten
			velocity.x = 0;
		else
			velocity.x /= 1.1f;
		
		if (gravityMultiplier == 0) {
			if (std::abs(velocity.y) < 0.0001f) 
				velocity.y = 0;
			else
				velocity.y /= 1.1f;
		}
	}

	Rigidbody::CollisionSide Rigidbody::getCollisionSide(const GameObject* other) const {
		Sprite* spr = getSprite(); 
		const int range = 15; // Räckvidden som objektens sidor måste ska vara från varandra för att det ska betraktas som en kollision. Testades för att få fram ett bra värde 

		// Y-led
		int top = spr->getY();
		int bottom = top + spr->getH();
		int otherTop = other->getY();
		int otherBottom = otherTop + other->getH();

		if (bottom - otherTop < range)
			return CollisionSide::BOTTOM; 
		if (top - otherBottom < range)
			return CollisionSide::TOP; 

		// X-led
		int left = spr->getX(); 
		int right = left + spr->getW();
		int otherLeft = other->getX();
		int otherRight = otherLeft + other->getW(); 

		if (right - otherLeft < range)
			return CollisionSide::RIGHT;
		if (left - otherRight < range)
			return CollisionSide::LEFT; 

		return CollisionSide::NONE; // Ingen kollision 
	}

	void Rigidbody::collided(GameObject* other) // Kallas från Collider-klassen när två objekt börjar kollidera 
	{
		if (isTrigger)
			return;
		static Uint32 timeLastBounceY = 0;
		static Uint32 timeLastBounceX = 0;
		const int deltaTimeToStopBounce = 50; // I ms

		switch (getCollisionSide(other))
		{
		case CollisionSide::BOTTOM: // De gör samma sak så ingen break 
		case CollisionSide::TOP:
			if (elasticityMultiplier > 0)  // Om elastisitet 
				if (SDL_GetTicks() - timeLastBounceY < deltaTimeToStopBounce)  // För att förhindra jitter sätts velocity till 0 om figuren "studsar" fort
					velocity.y = 0;
				else  // Annars om figuren ska studsa 
					velocity.y *= elasticityMultiplier * -1; // Vänder håll 
			else // Om inte elastisitet 
				velocity.y = 0; // Stanna figuren 
			timeLastBounceY = SDL_GetTicks();
			break;

		case CollisionSide::RIGHT: // Samma som ovan fast x-axeln 
		case CollisionSide::LEFT:
			if (elasticityMultiplier > 0)
				if (SDL_GetTicks() - timeLastBounceX < deltaTimeToStopBounce)
					velocity.x = 0;
				else 
					velocity.x *= elasticityMultiplier * -1; 
			else
				velocity.x = 0;
			timeLastBounceX = SDL_GetTicks();
			break;
		default: // Ingen kollision 
			return;
		}
	}

	void Rigidbody::move(std::vector<GameObject*>& objs)
	{
		Sprite* spr = getSprite();
		Vector oldPos(spr->getPos());

		Vector newPos(spr->getPos());
		newPos += velocity * Session::getDeltaTime();
		if (newPos == oldPos) // Om ingen förlyttning skett dvs velocity = 0 för x och y 
			return; 

		Collider* coll = spr->getComponent<Collider>();

		if (!coll || isTrigger) 
			spr->setPos(newPos);
		else
			checkIfMoveIsPossible(newPos, oldPos, coll); // Om collider måste det kollas så man faktiskt kan förlytta sig och inte kolliderar med nåt i den riktning man vill röra sig 
	}

	void Rigidbody::checkIfMoveIsPossible(Vector& newPos, Vector& oldPos, Collider* coll)
	{
		Sprite* spr = getSprite(); 
		 
		std::unordered_map<GameObject*, SDL_Rect> oldInterRects; // Sparar alla gamla intersect rects med tillhörande gameobject 
		for (auto obj : coll->collidingObjects) {
			SDL_Rect rect = spr->getRect();
			SDL_Rect otherRect = obj->getRect();
			SDL_Rect interRect;
			SDL_IntersectRect(&rect, &otherRect, &interRect);
			oldInterRects.insert(std::make_pair(obj, interRect));
		}

		spr->setPos(newPos); // Flyttar figuren 

		std::unordered_map<GameObject*, SDL_Rect> newInterRects;
		for (auto obj : coll->collidingObjects) { // Hämtar alla nya interrects med tillhörande gameobject 
			SDL_Rect rect = spr->getRect();
			SDL_Rect otherRect = obj->getRect();
			SDL_Rect interRect;
			SDL_IntersectRect(&rect, &otherRect, &interRect);
			newInterRects.insert(std::make_pair(obj, interRect));
		}

		for (auto& [obj, rectBefore] : oldInterRects) { // Jämför alla gamla intersect rects med de nya
			SDL_Rect rectAfter = newInterRects[obj];
			if (rectBefore.h < rectAfter.h) { // Om höjden på den nya är större betyder det att man försökte röra sig mot en figur man kolliderar med och därav skulle hamna inuti den 
				spr->setY(oldPos.y); // Flyttar då tillbaka y-pos
				velocity.y = 0; // Och sätter hastigheten i det ledet till 0 
			}
			if (rectBefore.w < rectAfter.w) { // Samma som ovan fast bredd/x-pos 
				spr->setX(oldPos.x);
				velocity.x = 0;
			}
		}
	}
}
