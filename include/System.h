#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace engine {
	class System
	{
	public:
		System();

		SDL_Renderer* getRen() const { return ren; }
		SDL_Window* getWindow() const { return win; }

		TTF_Font* getFont() const { return font; }

		int getStdFontSize() { return stdFontSize; }

		~System();

	private:
		SDL_Window* win;
		SDL_Renderer* ren;
		TTF_Font* font; 

		int stdFontSize = 16; 
	};

	extern System sys;
}

#endif