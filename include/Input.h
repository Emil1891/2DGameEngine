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

		/// <summary> Kollar om en viss tangent är nedtryckt. </summary>
		/// <param name="code"> Tangenten som ska kollas om den är nedtryckt i form av SDL_SCANCODE_TANGENT. </param>
		/// <returns> True alla frames som tangenten är nedtryckt. </returns>
		static bool getKeyDown(SDL_Scancode keyCode);

		/// <summary> Kollar om en viss tangent släpps/slutar tryckas ned. </summary>
		/// <param name="keyCode"> Tangenten som ska kollas om den släpps i form av SDL_SCANCODE_TANGENT. </param>
		/// <returns> True den frame som tangenten slutar tryckas ned/släpps. </returns>
		static bool getKeyUp(SDL_Scancode keyCode);

		/// <summary> Kollar om en viss musknapp blir nedtryckt. </summary>
		/// <param name="code"> Knappen som blir nedtryckt där 1 = vänster, 2 = mitten, 3 = höger. (Kan också använda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True den frame som knappen blir nedtryckt. </returns>
		static bool getMouseButton(int button);

		/// <summary> Kollar om en viss musknapp är nedtryckt. </summary>
		/// <param name="code"> Knappen som är nedtryckt där 1 = vänster, 2 = mitten, 3 = höger. (Kan också använda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True alla frames som knappen blir nedtryckt. </returns>
		static bool getMouseButtonDown(int button);

		/// <summary> Kollar om en viss musknapp släpps/slutar tryckas ned. </summary>
		/// <param name="keyCode"> Musknappen som ska kollas om den släpps där 1 = vänster, 2 = mitten, 3 = höger. (Kan också använda SDL_BUTTON_LEFT/MIDDLE/RIGHT). </param>
		/// <returns> True den frame som musknappen slutar tryckas ned/släpps. </returns>
		static bool getMouseButtonUp(int button); 

		/// <summary> Kan användas för att få musens position. Positionen uppdateras en gång per frame. </summary>
		/// <returns> Musens X-koordinat i relation till spelfönstret. </returns>
		static int getMousePosX() { return mouseX; }

		/// <summary> Kan användas för att få musens position. Positionen uppdateras en gång per frame. </summary>
		/// <returns> Musens Y-koordinat i relation till spelfönstret. </returns>
		static int getMousePosY() { return mouseY; }

		static Vector getMousePos() { return Vector(mouseX, mouseY); }

		/// <summary> Återställer input, används vid t.ex byte av scen. </summary>
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