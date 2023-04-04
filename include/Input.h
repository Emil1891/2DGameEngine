#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <unordered_set>
#include "Vector.h"

namespace engine {
	class Input 
	{
	public:
		static void updateInput();

		/// <summary> Kollar om en viss tangent blir nedtryckt. </summary>
		/// <param name="code"> Tangenten som ska kollas om den blir nedtryckt i form av SDL_SCANCODE_TANGENT. </param>
		/// <returns> True den frame som tangenten blir nedtryckt. </returns>
		static bool getKey(SDL_Scancode keyCode);

		/// <summary> Kollar om en viss tangent �r nedtryckt. </summary>
		/// <param name="code"> Tangenten som ska kollas om den �r nedtryckt i form av SDL_SCANCODE_TANGENT. </param>
		/// <returns> True alla frames som tangenten �r nedtryckt. </returns>
		static bool getKeyDown(SDL_Scancode keyCode);

		/// <summary> Kollar om en viss tangent sl�pps/slutar tryckas ned. </summary>
		/// <param name="keyCode"> Tangenten som ska kollas om den sl�pps i form av SDL_SCANCODE_TANGENT. </param>
		/// <returns> True den frame som tangenten slutar tryckas ned/sl�pps. </returns>
		static bool getKeyUp(SDL_Scancode keyCode);

		/// <summary> Kollar om en viss musknapp blir nedtryckt. </summary>
		/// <param name="code"> Knappen som blir nedtryckt d�r 1 = v�nster, 2 = mitten, 3 = h�ger. (Kan ocks� anv�nda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True den frame som knappen blir nedtryckt. </returns>
		static bool getMouseButton(int button);

		/// <summary> Kollar om en viss musknapp �r nedtryckt. </summary>
		/// <param name="code"> Knappen som �r nedtryckt d�r 1 = v�nster, 2 = mitten, 3 = h�ger. (Kan ocks� anv�nda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True alla frames som knappen blir nedtryckt. </returns>
		static bool getMouseButtonDown(int button);

		/// <summary> Kollar om en viss musknapp sl�pps/slutar tryckas ned. </summary>
		/// <param name="keyCode"> Musknappen som ska kollas om den sl�pps d�r 1 = v�nster, 2 = mitten, 3 = h�ger. (Kan ocks� anv�nda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True den frame som musknappen slutar tryckas ned/sl�pps. </returns>
		static bool getMouseButtonUp(int button); 

		/// <summary> Kan anv�ndas f�r att f� musens position. Positionen uppdateras en g�ng per frame. </summary>
		/// <returns> Musens X-koordinat i relation till spelf�nstret. </returns>
		static int getMousePosX() { return mouseX; }

		/// <summary> Kan anv�ndas f�r att f� musens position. Positionen uppdateras en g�ng per frame. </summary>
		/// <returns> Musens Y-koordinat i relation till spelf�nstret. </returns>
		static int getMousePosY() { return mouseY; }

		static Vector getMousePos() { return Vector(mouseX, mouseY); }

		/// <summary> �terst�ller input, anv�nds vid t.ex byte av scen. </summary>
		static void reset(); 

		Input() = delete; // Ska inte kunna skapa instanser av denna klass 

	private:
		inline static int mouseX = 0; 
		inline static int mouseY = 0; 

		inline static std::unordered_set<SDL_Scancode> prevFrameKeys, releasedKeys, keysClickedThisFrame;
		inline static std::unordered_set<int> prevFrameMouseButtons, releasedMouseButtons, mouseButtonsClickedThisFrame;

		inline static const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		inline static Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		static void updateSets();
	};
}

#endif