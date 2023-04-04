#include "Session.h"
#include "MainMenu.h"
#include <SDL2/SDL_mixer.h>

int main(int argc, char** argv) {
	Mix_Volume(-1, 8);
	engine::Session::run(MainMenu::getInstance(), 60); 
	return 0; 
}
