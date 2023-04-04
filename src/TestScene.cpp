#include "TestScene.h"
#include "Engine.h"
#include "TestAlien.h"
#include "MainMenu.h"

using namespace engine; 

Sprite* createSlime(int x, int y) {
	Sprite* slime = Sprite::getInstance(x, y, "slime.png");
	slime->addComponent<Collider>();
	slime->setName("Slime");

	return slime; 
}

static Text* createInstructionText() {
	Text* text = Text::getInstance(10, 10, "Dra alien figuren med musen och testa kollidera med slimen. Blir rod vid kollision.", 500);
	text->setFontSize(25);
	return text;
}

static void loadMainMenu() {
	TestScene::getInstance()->loadScene(MainMenu::getInstance());
}

static Button* createMainMenuButton() {
	Button* btn = Button::getInstance(800, 30, "Main Menu");
	btn->setSize(btn->getW() / 3, btn->getH() / 3);
	btn->setOnClick(loadMainMenu);
	return btn;
}

static ParticleSystem* createPs() {
	ParticleSystem* ps = ParticleSystem::getInstance(850, 500, { "slime.png", "slimeBlue.png", "slimeGreen.png" });
	ps->setDecreaseSizeOverTime(2);
	ps->setSpawnDelay(15);
	ps->setFadeOut(2);
	ps->setParticleVelocityY(-1);
	ps->setParticleVelocityX(0.1);
	return ps;
}

void TestScene::enter()
{
	TestAlien* alien = TestAlien::getInstance(100, 350);

	Sprite* slime = createSlime(500, 500); 
	Text* intructions = createInstructionText();
	Button* mainMenuBtn = createMainMenuButton();

	TextField* tf = TextField::getInstance(750, 250, "Testa skriv...");
	tf->setName("TextField");

	ParticleSystem* ps = createPs();
	Text* psText = Text::getInstance(750, 600, "Valdigt enkel partikeleffekt for att visa som ett exempel", 250);
	psText->setFontSize(20);

	add({ alien, slime, intructions, mainMenuBtn, tf, ps, psText });
}
