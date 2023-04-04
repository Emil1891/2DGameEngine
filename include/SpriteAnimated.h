#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include <string>
#include <vector>

#include "Sprite.h"

namespace engine {
	class SpriteAnimated : public Sprite 
	{
	public: 
		/// <summary> Skapar en animerad Sprite baserat p� de givna bilderna. </summary>
		/// <param name="x"> X pos </param>
		/// <param name="y"> Y pos </param>
		/// <param name="imageFileNames"> Filnamnen p� alla bilder i ordning och bilderna m�ste vara placerade i .../images/ mappen. </param>
		/// <returns> Pekare till animerad sprite. </returns>
		static SpriteAnimated* getInstance(int x, int y, std::initializer_list<std::string> imageFileNames, const bool playOnce = false); 

		/// <summary> Best�mmer hur snabbt animationen ska spela d�r bashastigheten multipliceras med speed. OBS att det kan limiteras av FPS. </summary>
		/// <param name="speed"> Snabbhet, h�gre v�rde = snabbare. </param>
		void setAnimationSpeed(float speed) { animationSpeed = speed; }

		/// <summary> Best�mmer om bildstorleken ska uppdateras till den nya bilden i animationen eller om alla bilder ska vara lika stora. </summary>
		/// <param name="toggle"> true = bildernas storlek uppdateras. false = alla bilder har samma storlek. Default: false </param>
		void setUpdateSizeOnImageUpdate(const bool toggle) { updateSizeOnImageUpdate = toggle; }

		void engineUpdate(std::vector<GameObject*>) override;

		/// <summary> Slutar animera och visar ist�llet den f�rsta bilden som angavs till konstruktorn vid skapande av objektet. F�r att visa annan bild, anv�nd changeImage(...) efter du pausat.
		///  OBS att changeImage(...) inte p�verkar animationen om den �terupptas. F�r att �teruppta animationen, anv�nd playAnimation(). Om redan pausat h�nder inget. </summary>
		void pauseAnimation();

		/// <summary> Forts�tter spela upp animationen fr�n b�rjan om det var tidigare pausat. Var det inte pausat tidigare h�nder inget. </summary>
		void playAnimation() { paused = false; }

		bool isAnimationPlaying() const { return !paused; }

		/// <summary> �ndrar animationen som spelar genom att ers�tta bilderna som animationen best�r av. Den nya animationen b�rjar fr�n b�rjan. </summary>
		/// <param name="imageFileNames"> Filnamnen p� alla bilder i ordning och bilderna m�ste vara placerade i .../images/ mappen. </param>
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

		const int msBetweenAnimUpdate = 50; // Standardv�rde som divideras med speed f�r att best�mma animationens hastighet  

		Uint32 startTime = 0; // Variabel som uppdateras varje g�ng bilden uppdateras till nuvarande tiden: SDL_GetTicks() 
	};
}

#endif