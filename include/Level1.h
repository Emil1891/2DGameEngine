#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"

class Level1 : public engine::Scene
{
public:

	static Level1* getInstance() {
		static Level1* scene = new Level1();
		return scene;
	}

	virtual void enter() override;

	bool isPaused() const { return paused; }
	void setPaused(const bool toggle) { paused = toggle; }

private:
	Level1() {}

	bool paused = false;
};

#endif