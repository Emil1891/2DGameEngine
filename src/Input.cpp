#include "Input.h"
#include <iostream>

namespace engine {

	// Tar bort gamla v�rden och uppdaterar vilka knappar/tangenter som sl�ppts, knapparna som efters�ks l�ggs till i get/getUp funktionerna
	void Input::updateSets()  
	{
		for (auto k : keysClickedThisFrame)
			prevFrameKeys.insert(k);
		for (auto m : mouseButtonsClickedThisFrame)
			prevFrameMouseButtons.insert(m); 

		keysClickedThisFrame.clear(); 
		mouseButtonsClickedThisFrame.clear(); 
		releasedKeys.clear();
		releasedMouseButtons.clear(); 

		auto keyIt = prevFrameKeys.begin();
		while (keyIt != prevFrameKeys.end()) {
			if (!currentKeyStates[*keyIt]) { // Om tangenten f�rra framen inte l�ngre �r nedtryckt
				releasedKeys.insert(*keyIt);
				keyIt = prevFrameKeys.erase(keyIt); // Ta bort fr�n set
			}
			else
				++keyIt;
		}

		auto mouseIt = prevFrameMouseButtons.begin();
		while (mouseIt != prevFrameMouseButtons.end()) {
			if (mouseState != SDL_BUTTON(*mouseIt)) { // Om knappen f�rra framen inte l�ngre �r nedtryckt
				releasedMouseButtons.insert(*mouseIt); 
				mouseIt = prevFrameMouseButtons.erase(mouseIt); // Ta bort fr�n set
			}
			else
				++mouseIt;
		}
	}

	void Input::updateInput()
	{
		mouseState = SDL_GetMouseState(&mouseX, &mouseY); 
		updateSets();
	}

	bool Input::getKey(SDL_Scancode keyCode)
	{
		if (prevFrameKeys.contains(keyCode)) // Om tangenten redan var nedtryckt framen innan 
			return false;
		else if (currentKeyStates[keyCode]) { // Annars kolla om tangenten �r nedtryckt
			keysClickedThisFrame.insert(keyCode);
			return true;
		}
		return false;
	}

	bool Input::getKeyDown(SDL_Scancode keyCode)
	{
		return currentKeyStates[keyCode];
	}

	bool Input::getKeyUp(SDL_Scancode keyCode)
	{
		if (currentKeyStates[keyCode]) 
			keysClickedThisFrame.insert(keyCode);
		return releasedKeys.contains(keyCode);
	}

	bool Input::getMouseButton(int button)
	{
		if (prevFrameMouseButtons.contains(button)) // Om knappen var nedtryckt framen innan 
			return false;
		else if (mouseState == SDL_BUTTON(button)) { // Annars kolla om knappen �r nedtryckt
			mouseButtonsClickedThisFrame.insert(button);
			return true;
		}
		return false;
	}

	bool Input::getMouseButtonDown(int button)
	{
		return mouseState == SDL_BUTTON(button);
	}

	bool Input::getMouseButtonUp(int button)
	{
		if (mouseState == SDL_BUTTON(button)) 
			mouseButtonsClickedThisFrame.insert(button);
		return releasedMouseButtons.contains(button);
	}

	void Input::reset() {
		prevFrameKeys.clear();
		releasedKeys.clear();
		keysClickedThisFrame.clear();
		prevFrameMouseButtons.clear();
		releasedMouseButtons.clear();
		mouseButtonsClickedThisFrame.clear(); 
	}
}