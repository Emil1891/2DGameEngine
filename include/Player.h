#ifndef PLAYER_h
#define PLAYER_h

#include "Engine.h"

class Player : public engine::Sprite
{
public:
	static Player* getInstance(int x, int y);

	void update() override;

	void onCollisionStay(GameObject* collision) override;

private:
	Player(int x, int y);

	enum class Direction {
		UP, RIGHT, DOWN, LEFT
	};

	Direction currentDir = Direction::RIGHT; // Spelaren spawnar med riktning åt höger 

	void move();
	void fire(); 
	void spawnBullet() const; 
	engine::Vector getBulletSpawnPos() const; 
	engine::Vector getBulletForce() const;

	void takeDamage(); 
	void gameOver(); 

	const float moveSpeed = 0.015f; 
	int health = 5; 

	engine::Collider* coll = 0; // Dessa tas bort i sprite eftersom de är sprite components 
	engine::Rigidbody* rb = 0;
};

#endif