#include "AudioManager.h"
#include "Constants.h"
#include "Input.h"

#include <iostream>

namespace engine {

	void AudioManager::playSound(const std::string fileName, const int loops)
	{
		Mix_Chunk* sound = Mix_LoadWAV((constants::gResPath + "sounds/" + fileName).c_str());
		if (sound == NULL) {
			std::cout << "Kunde inte ladda ljudfil: " << fileName << std::endl;
			return;
		}
		channelsUsed.insert(Mix_PlayChannel(-1, sound, loops));
	}

	void AudioManager::closeFinishedChannels() {
		auto it = channelsUsed.begin(); 
		while (it != channelsUsed.end()) {
			if (!Mix_Playing(*it)) { // Slutat spela 
				Mix_Chunk* chunk = Mix_GetChunk(*it);
				if (chunk) 
					Mix_FreeChunk(chunk);
				it = channelsUsed.erase(it);
			}
			else
				++it; 
		}
	}

	void AudioManager::updateAudio()
	{
		closeFinishedChannels();
		// Sänka volym = F5, höja volym = F6 
		if (Input::getKey(SDL_SCANCODE_F5))
			Mix_Volume(-1, Mix_Volume(-1, -1) - 1);
		if (Input::getKey(SDL_SCANCODE_F6))
			Mix_Volume(-1, Mix_Volume(-1, -1) + 1);
	}

}