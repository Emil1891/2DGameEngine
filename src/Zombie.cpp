#include "Zombie.h"
#include "Level1UI.h"
#include "GameManager.h"
#include "Level1.h"

#include <cmath>

using namespace engine; 

Zombie* Zombie::getInstance(int x, int y)
{
    return new Zombie(x, y);
}

Zombie::Zombie(int x, int y) : Sprite(x, y, "zombie.png") {
    addComponent<Collider>(); 
    rb = addComponent<Rigidbody>();
    rb->setGravityMultiplier(0);
    setName("Zombie");
    setStopAtWindowBorder(true);
}

void Zombie::update()
{
    if (Level1::getInstance()->isPaused())
        return;
    rotate();
    move(); 
}

void Zombie::doHarm()
{
    if (--health <= 0)
        killMe();
}

static void spawnBloodSplatter(const GameObject* zombie, const std::initializer_list<std::string>& bloodFiles) {
    const int size = 35;
    int midX = zombie->getX() + zombie->getW() / 2;
    int midY = zombie->getY() + zombie->getH() / 2;
    SpriteAnimated* blood = SpriteAnimated::getInstance(midX, midY, bloodFiles, true); 
    blood->setSize(size, size); 
    currentScene->add(blood);
}

void Zombie::killMe() {
    AudioManager::playSound("ZombieDamage.ogg");
    GameManager::lowerEnemyCount();
    spawnBloodSplatter(this, bloodFiles);
    currentScene->remove(this);
}

void Zombie::rotate() {
    Vector playerPos = currentScene->find("Player")->getPos();

    const float PI = 3.141592654;

    Vector myPos = getPos(); 
    float angle = std::atan2(myPos.y - playerPos.y, myPos.x - playerPos.x) * 180 / PI;
    // int offset = 180; // Behövdes ej någon offset 
    setRotation(angle - 180);
}

void Zombie::move() {
    Vector playerPos = currentScene->find("Player")->getPos();
    Vector dir = playerPos - getPos(); 

    rb->addForce(dir.normalized() * moveSpeed);
}
