#ifndef MAINMENU_H
#define MAINMENU_H

#include "Scene.h"

class MainMenu :  public engine::Scene
{
public:
	static MainMenu* getInstance() {
		static MainMenu* menu = new MainMenu();
		return menu;
	}

	virtual void enter() override;

private:
	MainMenu() {}
};

#endif