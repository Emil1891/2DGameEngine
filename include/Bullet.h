#ifndef BULLET_H
#define BULLET_H

#include "Engine.h"

class Bullet : public engine::Sprite
{
public:

	static Bullet* getInstance(int x, int y, engine::Vector velocity, const int playerRotation);

	void update() override;

	void onCollisionEnter(engine::GameObject* collision) override;

private:
	Bullet(int x, int y, engine::Vector velocity, const int playerRotation);
};

#endif