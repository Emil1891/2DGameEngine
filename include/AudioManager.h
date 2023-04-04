#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <SDL2/SDL_mixer.h>
#include <unordered_set>

namespace engine {
	class AudioManager
	{
	public:

		// OBS att SDL verkar ha problem med .wav filer s� undvik de. �r loops -1 s� loopas den o�ndligt
		static void playSound(const std::string fileName, const int loops = 0);

		static void updateAudio();  // Kallas fr�n spelloopen 

		AudioManager() = delete;

	private:
		static inline std::unordered_set<int> channelsUsed; 

		static void closeFinishedChannels(); 
	};
}

#endif