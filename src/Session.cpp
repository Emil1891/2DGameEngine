#include <SDL2/SDL.h>

#include "Session.h"
#include "Input.h"
#include "Scene.h"
#include "System.h"
#include "AudioManager.h"

namespace engine {
	void Session::run(Scene* startScene, const int FPS) { // Spelloopen
		deltaTime = 1000.0 / FPS; 

		bool quit = false;
		Scene* loadedScene = startScene;  
		currentScene = loadedScene;
		loadedScene->enter(); 

		Uint32 tickInterval = 1000 / FPS;
		Uint32 lastFrameTime = 0; 

		while (!quit) {	
			deltaTime = SDL_GetTicks() - lastFrameTime; // Tidskillnaden mellan frames i ms
			lastFrameTime = SDL_GetTicks(); 
			Uint32 nextTick = SDL_GetTicks() + tickInterval;

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT: 
					quit = true; 
					break;
				default: 
					loadedScene->handleEvent(event); // Knappar och textinmatningsfält 
				} 
			} 

			// Input
			Input::updateInput();

			// Frigör ljud osv.
			AudioManager::updateAudio(); 

			// Update (och engineUpdate()) 
			loadedScene->update(); 

			// Borttagna/tillagda objekt under körning 
			loadedScene->updateAddedRemovedObjects(); 

			// Kollar om det är dags att byta scen 
			loadedScene = loadedScene->changeScene(); 

			// Rendera alla objekt 
			SDL_SetRenderDrawColor(sys.getRen(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
			SDL_RenderClear(sys.getRen());
			loadedScene->render(); 

			SDL_RenderPresent(sys.getRen());

			// Ev. delay för att nå efterfrågad FPS 
			int delay = nextTick - SDL_GetTicks();
			if (delay > 0)
				SDL_Delay(delay);
		} 
	}
}