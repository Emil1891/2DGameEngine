#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Constants.h" 
#include "System.h"

namespace engine {
	System::System() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			std::cout << SDL_GetError(); 
		TTF_Init(); 
		win = SDL_CreateWindow("Zombie Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, 0);
		ren = SDL_CreateRenderer(win, -1, 0);
		Mix_OpenAudio(20050, AUDIO_S16SYS, 2, 4096);
		Mix_AllocateChannels(16);

		font = TTF_OpenFont((constants::gResPath + "fonts/arial.ttf").c_str(), stdFontSize); 
	}

	System::~System() {
		Mix_CloseAudio();
		TTF_CloseFont(font); 
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		TTF_Quit(); 
		SDL_Quit();
	}

	System sys;
}