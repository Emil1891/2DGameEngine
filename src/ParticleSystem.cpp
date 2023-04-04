#include "ParticleSystem.h"

namespace engine {

	// Particle system 
	ParticleSystem::ParticleSystem(int x, int y, std::string fileName) : GameObject(x, y), image(fileName)
	{
	}

	ParticleSystem::ParticleSystem(int x, int y, std::initializer_list<std::string> fileNames) : GameObject(x, y), images(fileNames)
	{
	}


	ParticleSystem* ParticleSystem::getInstance(int x, int y, std::string fileName)
	{
		return new ParticleSystem(x, y, fileName);
	}

	ParticleSystem* ParticleSystem::getInstance(int x, int y, std::initializer_list<std::string> fileNames)
	{
		return new ParticleSystem(x, y, fileNames);
	}

	void ParticleSystem::draw() const
	{
		for (auto p : spawnedParticles)
			p->draw(); 
	}

	void ParticleSystem::engineUpdate(std::vector<GameObject*> objs) 
	{
		for (auto p : spawnedParticles)
			p->engineUpdate(objs); // Uppdatera partiklarna (innan playing check:en eftersom jag vill att pause bara ska stoppa den från att spawna fler)

		if (!playing)
			return; 

		if (++framesSinceLastSpawn >= framesPerParticle) { // Kolla om det är dags att spawna ny partikel 
			framesSinceLastSpawn = 0; 
			if (images.empty()) { // Om man bara använder sig av en bild 
				spawnedParticles.push_back(new Particle(getX(), getY(), image, this));
			}
			else {
				spawnedParticles.push_back(new Particle(getX(), getY(), images[currentImageIndex++], this));
				if (currentImageIndex >= images.size()) // Kolla om out of bounds 
					currentImageIndex = 0; 
			}
		}
	}

	ParticleSystem::~ParticleSystem()
	{
		for (auto p : spawnedParticles)
			delete p;
	}

	// Particle
	ParticleSystem::Particle::Particle(int x, int y, std::string fileName, ParticleSystem* particleSystem) : Sprite(x, y, fileName), pSystem(particleSystem), 
		startLifeTime(SDL_GetTicks()), startAlpha(getColor().a), startW(getW()), startH(getH())
	{
		if (pSystem->particleStartW > 0) {
			setW(pSystem->particleStartW);
			startW = pSystem->particleStartW; 
		}
		if (pSystem->particleStartH > 0) {
			setH(pSystem->particleStartH);
			startH = pSystem->particleStartH;
		}
	}

	void ParticleSystem::Particle::engineUpdate(std::vector<GameObject*>) // Parametern används ej 
	{
		//Sprite::engineUpdate(objs); // Partiklarna kan inte ha komponenter just nu så behöver inte kalla på superklassen 
		if(!updateTime())
			return; 

		updatePosition(); 

		if (pSystem->fadeOutTime > 0)  // Om partikeln ska fade:a ut (> 0 ist. för  == 0 ty råkar tillämparen ange negativa tal blir det fel om man ändå försöker fade:a ut)
			updateFade(); 

		if (pSystem->sizeTime > 0) 
			updateSize(); 
	}

	bool ParticleSystem::Particle::updateTime()
	{
		lifeTime = SDL_GetTicks() - startLifeTime;
		if (lifeTime >= pSystem->maxLifeTime) {
			destroy();
			return false;
		}
		return true;
	}

	void ParticleSystem::Particle::updatePosition()
	{
		setX(getX() + pSystem->velX);
		setY(getY() + pSystem->velY);
	}

	void ParticleSystem::Particle::updateFade()
	{
		float percentLivedTime = (float)lifeTime / pSystem->fadeOutTime; // Ger hur länge den levt (relativt till fadeOutTime), alltså levt 90% ger 0.9. 50% = 0.5 osv. 
		int newAlpha = 0;
		if (percentLivedTime < 1) // Eftersom fadeOutTiden kan vara kortare än levnadstiden 
			newAlpha = startAlpha * (1 - percentLivedTime); // 1 - pLT så ger den hur mycket transparens som ska vara kvar 
		setColorAlpha(newAlpha);
	}

	void ParticleSystem::Particle::updateSize() // Samma som fade out ovan fast med storlek (ish)
	{
		float percentLivedTime = (float)lifeTime / pSystem->sizeTime;
		int newW = 0;
		int newH = 0;
		if (percentLivedTime < 1) {
			newW = startW * (1 - percentLivedTime);
			newH = startH * (1 - percentLivedTime);
		}
		setSize(newW, newH);
	}

	void ParticleSystem::Particle::destroy()
	{
		auto it = pSystem->spawnedParticles.begin();
		while (it != pSystem->spawnedParticles.end()) {
			if (*it == this) {
				pSystem->spawnedParticles.erase(it);
				break; 
			}
		}
		delete this; 
	}
}