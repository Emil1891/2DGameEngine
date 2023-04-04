#ifndef SESSION_H
#define SESSION_H

#include "Scene.h"

namespace engine {
	class Session
	{
	public:
		/// <summary> Kallas p� i main f�r att k�ra programmet/spelet. </summary>
		/// <param name="startScene"> den f�rsta scenen som ska visas. Skapas genom att �verskugga Scene-klassen. </param>
		/// <param name="FPS"> antalet FPS som spelet ska spela i. Rekommenderat: 60 (standard). �r FPS f�r h�gt s� k�r programmet s� fort det kan. </param>
		static void run(Scene* startScene, const int FPS = 60);

		static void setBgColor(const SDL_Color& color) { bgColor = color; }

		static float getDeltaTime() { return deltaTime; }

	private:
		inline static SDL_Color bgColor{ 255, 255, 255, 255 }; // Bakgrundsf�rgen 
		inline static float deltaTime = 16.7; // Tiden mellan frames i ms (s�tts och uppdateras varje frame i spelloopen) 

	};
}

#endif