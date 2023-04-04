#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"

class TestScene : public engine::Scene
{
public: 

	static TestScene* getInstance() {
		static TestScene* scene = new TestScene(); 
		return scene; 
	}

	virtual void enter() override;

private: 
	TestScene() {}
};

#endif