#include "Bullet.h"
#include "Zombie.h"

using namespace engine; 

Bullet* Bullet::getInstance(int x, int y, Vector velocity, const int playerRotation)
{
    return new Bullet(x, y, velocity, playerRotation);
}

Bullet::Bullet(int x, int y, Vector velocity, const int playerRotation) : Sprite(x, y, "bullet.png") {
    addComponent<Collider>(); 
    Rigidbody* rb = addComponent<Rigidbody>(); 
    rb->addForce(velocity); 
    rb->enableFriction(false); 
    rb->setIsTrigger(true); 
    rb->setGravityMultiplier(0); 
    setW(getW() * 2);
    setH(getH() * 2);
    setRotation(playerRotation + 90);
    currentScene->add(this);
}

bool isOutsideScreen(Vector pos) {
    if (pos.x < 0 || pos.y < 0)
        return true;

    int w, h;
    SDL_GetWindowSize(sys.getWindow(), &w, &h);
    if (pos.x > w || pos.y > h)
        return true; 

    return false;
}

void Bullet::update()
{
    if (isOutsideScreen(getPos())) 
        currentScene->remove(this);
}

void Bullet::onCollisionEnter(GameObject* collision)
{
    if (collision->getName() == "Player")
        return;

    if (collision->getName() == "Zombie") {
        if (Zombie* z = dynamic_cast<Zombie*>(collision))
            z->doHarm(); 
    }
    currentScene->remove(this); 
}
