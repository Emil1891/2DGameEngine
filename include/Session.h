#ifndef SESSION_H
#define SESSION_H

#include "Scene.h"

namespace engine {
	class Session
	{
	public:
		/// <summary> Kallas på i main för att köra programmet/spelet. </summary>
		/// <param name="startScene"> den första scenen som ska visas. Skapas genom att överskugga Scene-klassen. </param>
		/// <param name="FPS"> antalet FPS som spelet ska spela i. Rekommenderat: 60 (standard). Är FPS för högt så kör programmet så fort det kan. </param>
		static void run(Scene* startScene, const int FPS = 60);

		static void setBgColor(const SDL_Color& color) { bgColor = color; }

		static float getDeltaTime() { return deltaTime; }

	private:
		inline static SDL_Color bgColor{ 255, 255, 255, 255 }; // Bakgrundsfärgen 
		inline static float deltaTime = 16.7; // Tiden mellan frames i ms (sätts och uppdateras varje frame i spelloopen) 

	};
}

#endif