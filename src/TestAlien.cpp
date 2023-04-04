#include "TestAlien.h"
#include "Engine.h"
#include "TestScene.h"

using namespace engine;

TestAlien::TestAlien(int x, int y) : Sprite(x, y, "alien.png") {
	Collider* coll = addComponent<Collider>();
	coll->togglePixelCollisions(true); 
	setColor(0, 255, 0);
}

void TestAlien::update()
{
	if (TextField* tf = dynamic_cast<TextField*>(TestScene::getInstance()->find("TextField")))
		if (tf->isFocused()) 
			return;

	drag();
}

void TestAlien::onCollisionEnter(GameObject* collision)
{
	if (collision->getName() == "Slime")
		setColor(255, 0, 0);
}

void TestAlien::onCollisionExit(GameObject* collision)
{
	if (collision->getName() == "Slime")
		setColor(0, 255, 0);
}

void TestAlien::drag() // Drar alien med musen 
{
	static bool dragging = false;
	if (Input::getMouseButtonDown(1)) {
		SDL_Point p = { Input::getMousePosX(), Input::getMousePosY() };
		SDL_Rect rect = getRect();
		if (SDL_PointInRect(&p, &rect))
			dragging = true;
	}
	if (Input::getMouseButtonUp(1))
		dragging = false;

	if (dragging) 
		setPos(Input::getMousePosX() - (getW() / 2), Input::getMousePosY() - (getH() / 2));
}
