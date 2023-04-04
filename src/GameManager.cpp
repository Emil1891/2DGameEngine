#include "GameManager.h"
#include "Level1UI.h"
#include "Zombie.h"
#include "Level1.h"

#include "Engine.h"

#include <vector>
#include <random>
#include <cmath>

using namespace engine;

void GameManager::lowerEnemyCount()
{
	if (--enemiesLeft <= 0)
		startNewRound();
	else
		Level1UI::getInstance()->updateZombieCount(enemiesLeft); 
}

static bool spawnIsValid(const Vector& spawnPos, const std::vector<GameObject*>& spawnedZombies) {
	const int spawnRangeToZombies = 20; // Hur nära zombien får spawna andra zombies
	const int spawnRangeToPlayer = 100; // Hur nära zombien får spawna spelaren 

	GameObject* player = Level1::getInstance()->find("Player");
	Vector playerPos = Vector(500, 350); 
	if (player) // Första gången denna funktion kallas har inte spelaren lagts till i spelloopen
		playerPos = player->getPos(); 

	// Spelaren
	if (std::abs(spawnPos.x - playerPos.x) < spawnRangeToPlayer) // X-led
		return false; 
	if (std::abs(spawnPos.y - playerPos.y) < spawnRangeToPlayer) // Y-LED
		return false;

	// Andra zombies
	for (const auto& z : spawnedZombies) {
		if (std::abs(spawnPos.x - z->getX()) < spawnRangeToZombies) // X-led
			return false;
		if (std::abs(spawnPos.y - z->getY()) < spawnRangeToZombies) // Y-led
			return false;
	}
	return true;
}

static int spawnZombies(const int zombiesToSpawn) {
	int windowW, windowH;
	SDL_GetWindowSize(engine::sys.getWindow(), &windowW, &windowH);
	std::vector<GameObject*> spawnedZombies;
	int actualZombiesSpawned = 0;  

	for (int i = 0; i < zombiesToSpawn; i++) {
		int randomX, randomY;
		const int zombieWidth = 35;
		const int zombieHeight = 43; 
		int triesToSpawn = 0; 

		do {
			randomX = std::rand() % (windowW - zombieWidth + 1);  // Random ekvation: https://stackoverflow.com/a/7560151 
			randomY = std::rand() % (windowH - zombieHeight + 1);
			if (++triesToSpawn > 75) // Bryter loopen om det blir för många försök till spawn, dvs inga fler zombies får plats
				break;
		} while (!spawnIsValid(Vector(randomX, randomY), spawnedZombies)); 

		triesToSpawn = 0;
		++actualZombiesSpawned;
		Zombie* zombie = Zombie::getInstance(randomX, randomY);
		spawnedZombies.push_back(zombie);
		Level1::getInstance()->add(zombie);
	}
	return actualZombiesSpawned;
}

void GameManager::startNewRound(const bool newGame)
{
	if (newGame)
		roundNumber = 0;

	const int zombiesToSpawn = startEnemyCount + 1.5f * (++roundNumber - 1); // Liten funktion som avgör hur många fiender som ska spawna 

	enemiesLeft = spawnZombies(zombiesToSpawn); 
	Level1UI* UI = Level1UI::getInstance();
	UI->updateZombieCount(enemiesLeft); 
	UI->updateRoundText(roundNumber); 
}
