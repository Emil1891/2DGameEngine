#include "MainMenu.h"
#include "Engine.h"
#include "Level1.h"
#include "TestScene.h"

using namespace engine;

namespace {
	Button* createGenericButton(int x, int y, std::string text) { // Generisk knapp för just denna scen 
		Button* btn = Button::getInstance(x, y, text);
		btn->setSize(btn->getW() / 2, btn->getH() / 2);

		Text* btnText = btn->getText();
		btnText->setFont("ZombieFont.ttf");
		btnText->setFontSize(48);

		MainMenu::getInstance()->add(btn);
		return btn;
	}

	void onPlayClick() {
		MainMenu::getInstance()->loadScene(Level1::getInstance());
	}

	void createPlayButton() {
		Button* playButton = createGenericButton(381, 190, "Play");

		playButton->setOnClick(onPlayClick);
	}

	void showControls() {
		static Text* controlsText;
		bool exists = MainMenu::getInstance()->find("controlsText");
		if (!exists) { // Första gången den kallas så hittas den inte, skapar då texten  
			controlsText = Text::getInstance(30, 250, "W: move up \nA : move left \nS : move down \nD : move right \n\nLeft mouse button : shoot \n\nF5 : lower volume \nF6 : raise volume");
			controlsText->setName("controlsText");
			controlsText->setFontSize(25);
			MainMenu::getInstance()->add(controlsText);
		}
		else
			controlsText->setActive(!controlsText->isActive()); // Togglar den 
	}

	void createControlsButton() {
		Button* controlsButton = createGenericButton(381, 323, "Controls");
		controlsButton->setOnClick(showControls);
	}

	void loadTestLevel() {
		MainMenu::getInstance()->loadScene(TestScene::getInstance()); 
	}

	void createLoadTestLevelBtn() {
		Button* btn = createGenericButton(381, 449, "Test Level");
		btn->setOnClick(loadTestLevel);
	}

	void createTestLevelText() {
		Text* text = Text::getInstance(350, 551, "Test level to test/see things not in the game: text field, pixel collisions and particle system", 318);
		text->setFontSize(20);
		MainMenu::getInstance()->add(text);
	}
}

void MainMenu::enter()
{
	createPlayButton(); 
	createLoadTestLevelBtn();
	createControlsButton();
	createTestLevelText();

	Session::setBgColor(SDL_Color{ 200, 200, 200, 255 });

	Text* title = Text::getInstance(375, 50, "Zombie Game"); 
	title->setFont("ZombieFont.ttf");
	title->setFontSize(72); 
	add(title); 
}
