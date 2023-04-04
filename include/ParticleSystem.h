#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <string>

#include "Sprite.h"

namespace engine {
	class ParticleSystem : public GameObject
	{
	public:
		/// <summary> Skapar ett nytt partikelsystem med endast en bild. </summary>
		/// <param name="x"> x-koordinaten. </param>
		/// <param name="y"> y-koordinaten. </param>
		/// <param name="fileName"> filnamnet p� bilden som m�ste vara placerad i .../recources/images/ mappen. </param>
		/// <returns> pekare till det skapade partikelsystemet. </returns>
		static ParticleSystem* getInstance(int x, int y, std::string fileName);

		/// <summary> Skapar ett nytt partikelsystem med flera bilder. </summary>
		/// <param name="x"> x-koordinaten. </param>
		/// <param name="y"> y-koordinaten. </param>
		/// <param name="fileName"> filnamnen i form av {"filnamn.png", "..."} p� bilderna som m�ste vara placerade i .../recources/images/ mappen. </param>
		/// <returns> pekare till det skapade partikelsystemet. </returns>
		static ParticleSystem* getInstance(int x, int y, std::initializer_list<std::string> fileNames);

		void play() { playing = true; }
		void pause() { playing = false; }

		bool isPlaying() const { return playing; }

		/// <summary> Hur l�nge som en partikel ska "leva" tills den tas bort/f�rst�rs. </summary>
		/// <param name="maxLifeTime"> tiden i sekunder. Default:  2ms </param>
		void setParticleMaxLifeTime(float maxLifeTime) { this->maxLifeTime = maxLifeTime * 1000; }

		/// <summary> Best�mmer tiden som ska g� mellan att tv� partikar spawnar. </summary>
		/// <param name="framesPerParticle"> antalet frames som ska g� mellan att 2 partiklar ska spawna. 
		/// Standard �r 1 dvs 1 partikel varje frame. 2 skulle betyda 1 partikel varannan fram osv. </param>
		void setSpawnDelay(int framesPerParticle) { this->framesPerParticle = framesPerParticle; }

		/// <summary> G�r s� att partiklarna fade:ar ut, dvs �kar i transparens tills den �r helt borta. Default: 0 (fade:ar inte). </summary>
		/// <param name="time"> tiden det ska ta tills den blir helt transparent i sekunder. 0 = fade:ar inte. </param>
		void setFadeOut(float time) { fadeOutTime = time * 1000; }

		/// <summary> Best�mmer hur m�nga pixlar per frame som partikeln ska r�ra sig i x-led. Default: 0. </summary>
		/// <param name="speed"> hastigheten i antal pixlar per frame, +/- beroende p� �nskad riktning. OBS att ej hela pixlar t.ex 0.25 g�r att den flyttar 1 pixel var 4:e frame. </param>
		void setParticleVelocityX(float speed) { velX = speed; }

		/// <summary> Best�mmer hur m�nga pixlar per frame som partikeln ska r�ra sig i y-led. Default: 0. </summary>
		/// <param name="speed"> hastigheten i antal pixlar per frame, +/- beroende p� �nskad riktning. OBS positivt �r ned�t och att ej hela pixlar t.ex 0.25 g�r att den flyttar 1 pixel var 4:e frame.</param>
		void setParticleVelocityY(float speed) { velY = speed; }

		/// <summary> Tiden tills partikeln ska vara 0 i h�jd och bredd. Default 0 (minskar ej) </summary>
		/// <param name="time"> tiden i sekunder. 0 = �ndrar inte storlek. </param>
		void setDecreaseSizeOverTime(float time) { sizeTime = time * 1000; }

		/// <summary> S�tter storleken p� partiklarna vid spawn. </summary>
		/// <param name="w"> bredden. 0 = bildens "normala" storlek. </param>
		/// <param name="h"> h�jden. 0 = bildens "normala" storlek. </param>
		void setSize(int w, int h) override { particleStartW = w; particleStartH = h; }

		void draw() const override; 

		void engineUpdate(std::vector<GameObject*>) override;

		~ParticleSystem(); 

	protected:
		ParticleSystem(int x, int y, std::string fileName);
		ParticleSystem(int x, int y, std::initializer_list<std::string> fileNames);

	private:
		class Particle; 
		std::vector<Particle*> spawnedParticles;

		std::vector<std::string> images;
		int currentImageIndex = 0; 

		std::string image; 

		int maxLifeTime = 2000; // I ms
		bool playing = true; 

		float velX = 0; 
		float velY = 0; 

		int framesPerParticle = 1; 
		int framesSinceLastSpawn = 0; 
		int fadeOutTime = 0; 

		int particleStartW = 0; // 0 betyder bildens normala storlek 
		int particleStartH = 0;
		int sizeTime = 0; // Tiden tills partikeln ska vara 0, 0 i w, h (i ms)

		class Particle : public Sprite {
			Particle(int x, int y, std::string fileName, ParticleSystem* particleSystem);

			void engineUpdate(std::vector<GameObject*>) override; 
			bool updateTime(); 
			void updatePosition();
			void updateFade(); 
			void updateSize(); 

			void destroy(); // N�r lifetime >= maxLifeTime 

			int lifeTime = 0; // I ms
			int startLifeTime;
			int startAlpha;
			int startW;
			int startH;
			ParticleSystem* pSystem; 
			friend class ParticleSystem; 
		};
	};
}

#endif