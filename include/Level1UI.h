#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "Text.h"
#include "Button.h"

class Level1UI
{
public:
	static Level1UI* getInstance(const bool loadingNewLevel = false);

	void updateRoundText(const int round);
	void updateZombieCount(const int zombiesLeft); 

	void updateHealthText(const int hp);

	void showPausedUI(const bool toggle); 

	void showGameOver();

private: 
	Level1UI(); 

	// Standard UI
	engine::Text* roundText;
	engine::Text* zombieCountText; 
	engine::Text* playerHealthText;
	engine::Button* pauseButton;

	void createRoundText();
	void createZombieCountText();
	void createPlayerHealthText();
	void createPauseButton(); 

	// Pausmenyn
	engine::Button* continueButton;
	engine::Button* mainMenuButton;
	void createContinueButton();
	void createMainMenuButton();
};

#endif