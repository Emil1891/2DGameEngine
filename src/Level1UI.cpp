#include "Level1UI.h"
#include "Level1.h"
#include "GameManager.h"
#include "MainMenu.h"

#include <array>

using namespace engine; 

static std::string intToRoman(int num) // Source: https://www.c-sharpcorner.com/article/convert-numbers-to-roman-characters-in-c-sharp/ 
{
    std::string romanResult = "";
    std::array<std::string, 13> romanLetters = { "M", "CM","D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };

    std::array<int, 13> numbers = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };
    int i = 0;
    while (num != 0)
    {
        if (num >= numbers[i])
        {
            num -= numbers[i];
            romanResult += romanLetters[i];
        }
        else
        {
            i++;
        }
    }
    return romanResult;
}

static void pauseGame() {
    Level1::getInstance()->setPaused(true);
    Level1UI::getInstance()->showPausedUI(true); 
}

static void unPauseGame() {
    Level1::getInstance()->setPaused(false);
    Level1UI::getInstance()->showPausedUI(false);
}

static void loadMainMenu() {
    Level1::getInstance()->loadScene(MainMenu::getInstance());
}

static Button* createGenericPauseMenuButton(int x, int y, std::string text) { // Generisk knapp för denna scen 
    Button* btn = Button::getInstance(x, y, text);
    btn->setSize(btn->getW() / 2, btn->getH() / 2);
    btn->setActive(false);

    Text* btnText = btn->getText();
    btnText->setFont("ZombieFont.ttf");
    btnText->setFontSize(48);
    return btn;
}

Level1UI* Level1UI::getInstance(const bool loadingNewLevel) {
    static Level1UI* UI;
    if (loadingNewLevel) {
        delete UI;
        UI = new Level1UI();
    }
    return UI;
}

Level1UI::Level1UI() {
    createRoundText(); 
    createZombieCountText(); 
    createPlayerHealthText(); 
    createPauseButton(); 

    createContinueButton();
    createMainMenuButton();

    Level1::getInstance()->add({ roundText, zombieCountText, playerHealthText, pauseButton, continueButton, mainMenuButton });
}

void Level1UI::createRoundText() {
    roundText = Text::getInstance(30, 620, "I");
    roundText->setFont("RomanFont.otf");
    roundText->setFontSize(64);
}

void Level1UI::createZombieCountText() {
    std::string countText = "Enemies left: " + std::to_string(GameManager::getEnemyCount());
    zombieCountText = Text::getInstance(760, 625, countText);
    zombieCountText->setFont("ZombieFont.ttf");
    zombieCountText->setFontSize(45);
}

void Level1UI::createPlayerHealthText() {
    playerHealthText = Text::getInstance(30, 30, "Health: 5");
    playerHealthText->setFont("ZombieFont.ttf");
    playerHealthText->setFontSize(45);
}

void Level1UI::createPauseButton() {
    pauseButton = Button::getInstance(924, 15, "", "PauseButton.png");
    pauseButton->setSize(pauseButton->getW() / 4, pauseButton->getH() / 4);
    pauseButton->setOnClick(pauseGame);
}

void Level1UI::showPausedUI(const bool toggle) {
    pauseButton->setActive(!toggle);
    continueButton->setActive(toggle);
    mainMenuButton->setActive(toggle);
}

void Level1UI::showGameOver()
{
    Text* gameOverText = Text::getInstance(mainMenuButton->getText()->getX(), 240, "Game Over");
    gameOverText->setFont("ZombieFont.ttf");
    gameOverText->setFontSize(mainMenuButton->getText()->getFontSize());

    mainMenuButton->setActive(true);
    pauseButton->setActive(false);

    Level1::getInstance()->add(gameOverText);
}

void Level1UI::createContinueButton() {
    continueButton = createGenericPauseMenuButton(381, 240, "Continue");
    continueButton->setOnClick(unPauseGame);
}

void Level1UI::createMainMenuButton() {
    mainMenuButton = createGenericPauseMenuButton(381, 373, "Main Menu"); 
    mainMenuButton->setOnClick(loadMainMenu);
}

void Level1UI::updateRoundText(const int round)
{
    roundText->setText(intToRoman(round)); 
}

void Level1UI::updateZombieCount(const int zombiesLeft)
{
    std::string newText = "Enemies left: " + std::to_string(zombiesLeft);
    zombieCountText->setText(newText); 
}

void Level1UI::updateHealthText(const int hp) {
    playerHealthText->setText("Health: " + std::to_string(hp)); 
}
