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
		/// <param name="fileName"> filnamnet på bilden som måste vara placerad i .../recources/images/ mappen. </param>
		/// <returns> pekare till det skapade partikelsystemet. </returns>
		static ParticleSystem* getInstance(int x, int y, std::string fileName);

		/// <summary> Skapar ett nytt partikelsystem med flera bilder. </summary>
		/// <param name="x"> x-koordinaten. </param>
		/// <param name="y"> y-koordinaten. </param>
		/// <param name="fileName"> filnamnen i form av {"filnamn.png", "..."} på bilderna som måste vara placerade i .../recources/images/ mappen. </param>
		/// <returns> pekare till det skapade partikelsystemet. </returns>
		static ParticleSystem* getInstance(int x, int y, std::initializer_list<std::string> fileNames);

		void play() { playing = true; }
		void pause() { playing = false; }

		bool isPlaying() const { return playing; }

		/// <summary> Hur länge som en partikel ska "leva" tills den tas bort/förstörs. </summary>
		/// <param name="maxLifeTime"> tiden i sekunder. Default:  2ms </param>
		void setParticleMaxLifeTime(float maxLifeTime) { this->maxLifeTime = maxLifeTime * 1000; }

		/// <summary> Bestämmer tiden som ska gå mellan att två partikar spawnar. </summary>
		/// <param name="framesPerParticle"> antalet frames som ska gå mellan att 2 partiklar ska spawna. 
		/// Standard är 1 dvs 1 partikel varje frame. 2 skulle betyda 1 partikel varannan fram osv. </param>
		void setSpawnDelay(int framesPerParticle) { this->framesPerParticle = framesPerParticle; }

		/// <summary> Gör så att partiklarna fade:ar ut, dvs ökar i transparens tills den är helt borta. Default: 0 (fade:ar inte). </summary>
		/// <param name="time"> tiden det ska ta tills den blir helt transparent i sekunder. 0 = fade:ar inte. </param>
		void setFadeOut(float time) { fadeOutTime = time * 1000; }

		/// <summary> Bestämmer hur många pixlar per frame som partikeln ska röra sig i x-led. Default: 0. </summary>
		/// <param name="speed"> hastigheten i antal pixlar per frame, +/- beroende på önskad riktning. OBS att ej hela pixlar t.ex 0.25 gör att den flyttar 1 pixel var 4:e frame. </param>
		void setParticleVelocityX(float speed) { velX = speed; }

		/// <summary> Bestämmer hur många pixlar per frame som partikeln ska röra sig i y-led. Default: 0. </summary>
		/// <param name="speed"> hastigheten i antal pixlar per frame, +/- beroende på önskad riktning. OBS positivt är nedåt och att ej hela pixlar t.ex 0.25 gör att den flyttar 1 pixel var 4:e frame.</param>
		void setParticleVelocityY(float speed) { velY = speed; }

		/// <summary> Tiden tills partikeln ska vara 0 i höjd och bredd. Default 0 (minskar ej) </summary>
		/// <param name="time"> tiden i sekunder. 0 = ändrar inte storlek. </param>
		void setDecreaseSizeOverTime(float time) { sizeTime = time * 1000; }

		/// <summary> Sätter storleken på partiklarna vid spawn. </summary>
		/// <param name="w"> bredden. 0 = bildens "normala" storlek. </param>
		/// <param name="h"> höjden. 0 = bildens "normala" storlek. </param>
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

			void destroy(); // När lifetime >= maxLifeTime 

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