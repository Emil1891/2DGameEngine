#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Sprite.h"
#include "Engine.h"

class Zombie : public engine::Sprite
{
public:
	static Zombie* getInstance(int x, int y); 

	void update() override;

	void doHarm(); 

private:
	Zombie(int x, int y);

	void rotate(); 
	void move(); 
	void killMe(); 

	int health = 4; 

	float moveSpeed = 0.005f;

	engine::Rigidbody* rb = 0;

	// Blodet som kommer vid död 
	inline static const std::initializer_list<std::string> bloodFiles = { "blood/1.png", "blood/2.png", "blood/3.png", "blood/4.png", "blood/5.png", "blood/6.png", "blood/7.png",
		"blood/8.png", "blood/9.png", "blood/10.png", "blood/11.png", "blood/12.png", "blood/13.png" };
};

#endif