#ifndef TESTALIEN_H
#define TESTALIEN_H

#include "SpriteAnimated.h"

class TestAlien : public engine::Sprite
{
public:

	static TestAlien* getInstance(int x, int y) { return new TestAlien(x, y); }

	void update() override;

	void onCollisionEnter(GameObject* collision) override;
	void onCollisionExit(GameObject* collision) override;

private:
	TestAlien(int x, int y);

	void drag();
};

#endif