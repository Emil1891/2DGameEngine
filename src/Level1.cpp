#include "Level1.h"
#include "Player.h"
#include "Zombie.h"
#include "Level1UI.h"
#include "GameManager.h"

#include "Session.h"

void Level1::enter()
{
	paused = false; 
	engine::Session::setBgColor(SDL_Color{ 0, 150, 0, 255 });

	Player* player = Player::getInstance(500, 350); 

	add(player); 

	// UI
	Level1UI* UI = Level1UI::getInstance(true); // Behöver inte lägga till i scen eftersom den lägger till det som ska visas i sin konstruktor 

	GameManager::startNewRound(true); 
}
