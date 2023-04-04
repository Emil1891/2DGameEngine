#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include <string>
#include <vector>

#include "Sprite.h"

namespace engine {
	class SpriteAnimated : public Sprite 
	{
	public: 
		/// <summary> Skapar en animerad Sprite baserat på de givna bilderna. </summary>
		/// <param name="x"> X pos </param>
		/// <param name="y"> Y pos </param>
		/// <param name="imageFileNames"> Filnamnen på alla bilder i ordning och bilderna måste vara placerade i .../images/ mappen. </param>
		/// <returns> Pekare till animerad sprite. </returns>
		static SpriteAnimated* getInstance(int x, int y, std::initializer_list<std::string> imageFileNames, const bool playOnce = false); 

		/// <summary> Bestämmer hur snabbt animationen ska spela där bashastigheten multipliceras med speed. OBS att det kan limiteras av FPS. </summary>
		/// <param name="speed"> Snabbhet, högre värde = snabbare. </param>
		void setAnimationSpeed(float speed) { animationSpeed = speed; }

		/// <summary> Bestämmer om bildstorleken ska uppdateras till den nya bilden i animationen eller om alla bilder ska vara lika stora. </summary>
		/// <param name="toggle"> true = bildernas storlek uppdateras. false = alla bilder har samma storlek. Default: false </param>
		void setUpdateSizeOnImageUpdate(const bool toggle) { updateSizeOnImageUpdate = toggle; }

		void engineUpdate(std::vector<GameObject*>) override;

		/// <summary> Slutar animera och visar istället den första bilden som angavs till konstruktorn vid skapande av objektet. För att visa annan bild, använd changeImage(...) efter du pausat.
		///  OBS att changeImage(...) inte påverkar animationen om den återupptas. För att återuppta animationen, använd playAnimation(). Om redan pausat händer inget. </summary>
		void pauseAnimation();

		/// <summary> Fortsätter spela upp animationen från början om det var tidigare pausat. Var det inte pausat tidigare händer inget. </summary>
		void playAnimation() { paused = false; }

		bool isAnimationPlaying() const { return !paused; }

		/// <summary> Ändrar animationen som spelar genom att ersätta bilderna som animationen består av. Den nya animationen börjar från början. </summary>
		/// <param name="imageFileNames"> Filnamnen på alla bilder i ordning och bilderna måste vara placerade i .../images/ mappen. </param>
		void changeAnimation(std::initializer_list<std::string> imageFileNames); 

	protected: 
		SpriteAnimated(int x, int y, std::initializer_list<std::string> imageFiles, const bool playOnce = false);

	private: 
		std::vector<std::string> images; 
		float animationSpeed = 1; 
		bool paused = false;
		bool playOnce; 

		bool updateSizeOnImageUpdate = false;

		int currentImageIndex = 0; 

		const int msBetweenAnimUpdate = 50; // Standardvärde som divideras med speed för att bestämma animationens hastighet  

		Uint32 startTime = 0; // Variabel som uppdateras varje gång bilden uppdateras till nuvarande tiden: SDL_GetTicks() 
	};
}

#endif