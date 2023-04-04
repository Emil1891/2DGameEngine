#include "Player.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Level1UI.h"
#include "Level1.h"

#include <cmath>

using namespace engine;

Player* Player::getInstance(int x, int y)
{
	return new Player(x, y);
}

Player::Player(int x, int y) : Sprite(x, y, "player.png") {
	coll = addComponent<Collider>(); 
	rb = addComponent<Rigidbody>(); 
	rb->setGravityMultiplier(0); 
	setName("Player");
	setStopAtWindowBorder(true);
}

void Player::onCollisionStay(GameObject* collision)
{
	if (health <= 0)
		return;
	static Uint32 timeLastDamageTaken = 0; 
	int damageCooldown = 500; // I ms
	if (collision->getName() == "Zombie") {
		if (SDL_GetTicks() - timeLastDamageTaken > damageCooldown) {
			takeDamage();
			timeLastDamageTaken = SDL_GetTicks(); 
		}

	}
}

void Player::takeDamage() {
	Level1UI::getInstance()->updateHealthText(--health);
	AudioManager::playSound("PlayerDamage.ogg");

	if (health <= 0)
		gameOver(); 
}

void Player::gameOver() {
	Level1UI::getInstance()->showGameOver();
	Level1::getInstance()->setPaused(true);
}

void Player::update()
{
	if (Level1::getInstance()->isPaused())
		return;

	move(); 
	fire(); 
}

void Player::move()
{
	if (Input::getKeyDown(SDL_SCANCODE_W)) {
		rb->addForceY(-moveSpeed);
		currentDir = Direction::UP;
		setRotation(-90);
	}
	if (Input::getKeyDown(SDL_SCANCODE_A)) {
		rb->addForceX(-moveSpeed);
		currentDir = Direction::LEFT;
		setRotation(180);
	}
	if (Input::getKeyDown(SDL_SCANCODE_S)) {
		rb->addForceY(moveSpeed);
		currentDir = Direction::DOWN;
		setRotation(90);
	}	
	if (Input::getKeyDown(SDL_SCANCODE_D)) {
		rb->addForceX(moveSpeed);
		currentDir = Direction::RIGHT;
		setRotation(0); 
	}
}

void Player::fire()
{
	if (Input::getMouseButton(SDL_BUTTON_LEFT)) {
		spawnBullet(); 
		AudioManager::playSound("GunShot.ogg"); 
	}
}

Vector Player::getBulletSpawnPos() const {
	const float gunDistanceMultiplier = 0.6f; 
	const int bulletSizeLength = 12; 
	const int x = getX();
	const int y = getY(); 
	const int w = getW();
	const int h = getH(); 
	switch (currentDir) // Testades fram lite för att få det perfekt 
	{
	case Direction::RIGHT:
		return Vector(x + w, y + h * gunDistanceMultiplier);
	case Direction::LEFT:
		return Vector(x - bulletSizeLength, y + h * (1 - gunDistanceMultiplier) - bulletSizeLength);
	case Direction::UP:
		return Vector(x + w * (gunDistanceMultiplier + 0.07f), y - bulletSizeLength);
	case Direction::DOWN:
		return Vector(x + w * (1 - gunDistanceMultiplier - 0.1f), y + h);
	default:
		return Vector(0, 0); // Ska aldrig komma hit 
	}
}

Vector Player::getBulletForce() const {
	const int force = 2;
	switch (currentDir)
	{
	case Direction::RIGHT:
		return Vector(force, 0);
	case Direction::LEFT:
		return Vector(-force, 0);
	case Direction::UP:
		return Vector(0, -force);
	case Direction::DOWN:
		return Vector(0, force); 
	default:
		return Vector(0, 0); // Ska aldrig komma hit 
	}
}

void Player::spawnBullet() const {
	Vector pos = getBulletSpawnPos(); 
	Vector force = getBulletForce();
	Bullet::getInstance(pos.x, pos.y, force, getRotation()); 
}
