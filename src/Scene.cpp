#include "Scene.h"
#include "GameObject.h"
#include "Sprite.h"
#include "System.h"
#include "Button.h"
#include "Input.h"
#include "TextField.h"

#include <algorithm>

namespace engine {

	Scene* currentScene = nullptr; 

	void Scene::exit()
	{
		for (GameObject* gObj : gameObjects) 
				delete gObj;
		gameObjects.clear();
		added.clear(); // Added och removed b�r redan vara tomma men detta garanterar det 
		removed.clear();
	}

	void Scene::handleEvent(SDL_Event& event) // Knappar och textfields har unika funktioner som beh�ver kallas p� med SDL_Event 
	{
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN: 
			for (GameObject* gObj : gameObjects)
				if (Button* obj = dynamic_cast<Button*>(gObj))
					obj->mouseDown(event);
				else if (TextField* obj = dynamic_cast<TextField*>(gObj))
					obj->mouseDown(event); 
			break;
		case SDL_MOUSEBUTTONUP:
			for (GameObject* gObj : gameObjects)
				if (Button* b = dynamic_cast<Button*>(gObj))
					b->mouseUp(event);
				else if (TextField* obj = dynamic_cast<TextField*>(gObj))
					obj->mouseDown(event);
			break;
		case SDL_KEYDOWN:
			for (auto gObj : gameObjects)
				if (TextField* t = dynamic_cast<TextField*>(gObj))
					t->edit(event); 
			break; 
		case SDL_TEXTINPUT:
			for (auto gObj : gameObjects)
				if (TextField* t = dynamic_cast<TextField*>(gObj))
					t->write(event);
			break;
		} 
	}

	void Scene::add(GameObject* gObj) {
		added.push_back(gObj);
	}

	void Scene::add(std::initializer_list<GameObject*> list)
	{
		for (GameObject* obj : list) {
			added.push_back(obj);
		}
	}

	void Scene::remove(GameObject* gObj) { 
		if (std::find(removed.begin(), removed.end(), gObj) != removed.end()) // Om objektet redan efterfr�gats att tas bort: returnera 
			return;
		removed.push_back(gObj);
		for (auto& c : gObj->children) // Ta bort children 
			remove(c); 
	}

	GameObject* Scene::find(const std::string name) const
	{
		for (auto gObj : gameObjects)
			if (gObj->getName() == name)
				return gObj;
		return nullptr;
	}

	std::vector<GameObject*> Scene::findAll(const std::string name) const
	{
		std::vector<GameObject*> tempVec;
		for (auto gObj : gameObjects)
			if (gObj->getName() == name)
				tempVec.push_back(gObj);
		return tempVec;
	}

	void Scene::update()
	{
		for (GameObject* gObj : gameObjects)
			if (gObj->isActive()) {
				gObj->engineUpdate(gameObjects); // Spelmotor grejer (kollisioner osv)
				gObj->update(); // Till�mpningsgrejer 
			}
	}

	void Scene::render()
	{
		for (GameObject* gObj : gameObjects)
			if (gObj->isActive())
				gObj->draw();
	}

	void Scene::updateAddedRemovedObjects()
	{
		// Tillagda objekt 
		for (GameObject* gObj : added)
			gameObjects.push_back(gObj);
		added.clear();

		// Borttagna 
		for (GameObject* gObj : removed) {  
			for (auto i = gameObjects.begin(); i != gameObjects.end();)
				if (*i == gObj) 
					i = gameObjects.erase(i);
				else
					++i;
			delete gObj;
		}
		removed.clear(); 
	}

	Scene* Scene::changeScene()
	{
		if (nextScene == nullptr) { // Ska inte byta scen (loadScene har inte kallats)
			return currentScene;
		}
		currentScene->exit();
		nextScene->enter(); 
		currentScene = nextScene; 
		nextScene = nullptr; 
		Input::reset(); 
		return currentScene; 
	}
}