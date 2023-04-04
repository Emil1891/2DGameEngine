#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <iostream>

#include "Scene.h"
#include "Vector.h"

namespace engine {
	class GameObject
	{
	public:
		float getX() const { return actualPosX; }
		float getY() const { return actualPosY; }
		Vector getPos() const { return Vector(actualPosX, actualPosY); }
		int getW() const { return rect.w; }
		int getH() const { return rect.h; }

		// OBS att det avrundas till hela pixlar vid visning p� sk�rmen 
		void setX(float x);

		// OBS att det avrundas till hela pixlar vid visning p� sk�rmen 
		void setY(float y);

		// OBS att det avrundas till hela pixlar vid visning p� sk�rmen 
		void setPos(float x, float y) { setX(x); setY(y); }
		void setPos(Vector vec) { setPos(vec.x, vec.y); }

		// OBS att pixel kollisioner inte tar h�nsyn till �ndrade storlekar. De sker allts� p� originalstorleken. �ndra storlek i photoshop om pixel kollisioner �nskas. 
		// Rect koll. (�t b�da h�llen) fungerar bra. 
		void setW(int w);
		// OBS att pixel kollisioner inte tar h�nsyn till �ndrade storlekar. De sker allts� p� originalstorleken. �ndra storlek i photoshop om pixel kollisioner �nskas. 
		// Rect koll. (�t b�da h�llen) fungerar bra. 
		void setH(int h);
		virtual void setSize(int w, int h) { setW(w); setH(h); } // Virtual eftersom den �verskuggas i particlesystem 

		/// <summary> L�gger ett objekt som dess child som f�ljer med sin parent om den flyttas. Alla children till ett objekt tas ocks� bort om parent:en f�rst�rs </summary>
		/// <param name="obj"> Objektet som ska bli en child </param>
		void addChild(GameObject* obj); 

		/// <summary> Tar bort ett objekt som en child. </summary>
		/// <param name="obj"> Objektet som ska tas bort som child </param>
		/// <returns> Returnerar pekare till objektet som togs bort om det lyckades eller nullptr om det inte hittades. </returns>
		GameObject* removeChild(GameObject* obj);

		bool hasChildren() const { return !children.empty(); }

		bool isActive() const { return active; };
		/// <summary> Om objektet �r inaktivt s� kommer det inte att renderas alls. Ev. children s�tts ocks� inaktiva/aktiva. </summary>
		/// <param name="toggle"> Aktiv true/false </param>
		void setActive(const bool toggle);

		SDL_Rect getRect() const { return rect; }

		std::string getName() const { return name; }
		void setName(std::string name) { this->name = name; }

		// Kallas p� varje frame och ritar ut objektet 
		virtual void draw() const = 0;

		// Kallas p� varje frame
		virtual void update();

		/// <summary> S�tter funktionen som k�rs i update(), dvs varje frame. Funktionen kan skrivas direkt i en �verskuggad update 
		/// om man skapar en ny klass som �rver fr�n GameObject eller dess subklasser </summary>
		/// <param name="newFunction"> Tar emot en funktionspekare som returnerar void och inte tar n�gra argument </param>
		void setOnUpdate(void (*newFunction) ()) { updateMethodToCall = newFunction; }

		/// <summary> Roterar bilden. OBS att rektangeln som best�mmer bl.a klickyta INTE uppdateras och att den alltid �r en rektangel, s� rotationer (exkl. 180*n) g�r att kollisioner/klick 
		/// kan ske utanf�r bilden. F�r knappar och andra interagerbara f�rem�l b�r rotationen allts� ske i t.ex. photoshop innan den l�ggs in i spelet. </summary>
		/// <param name="rotation"> i antal grader. </param>
		void setRotation(int rotation); 
		int getRotation() const { return rotationAngle; }

		/// <summary> F�r att flippa bilden l�ngs x-axeln. Om redan flippad s� flippar den tillbaka. F�r att �terst�lla helt kan resetFlip() anv�ndas. </summary>
		void flipX();
		/// <summary> F�r att flippa bilden l�ngs x-axeln. Om redan flippad s� flippar den tillbaka. F�r att �terst�lla helt kan resetFlip() anv�ndas. </summary>
		void flipY(); 
		/// <summary> �terst�ller bildens flip p� b�da axlarna. </summary>
		void resetFlip();

		/// <summary> S�tter om det ska vara m�jligt att r�ra sig utanf�r f�nstret. Inaktivt fr�n b�rjan. Om aktivt g�rs det en check vid f�rflyttning (setX(), setY(), setPos(). </summary>
		/// <param name="toggle"> true = stoppar figuren fr�n att r�ra sig utanf�r. </param>
		void setStopAtWindowBorder(const bool toggle) { stopAtWindowBorder = toggle; }
		bool getStopAtWindowBorder() const { return stopAtWindowBorder; }

		virtual ~GameObject();

		// Anv�nds bara av spelmotorn f�r objekt som beh�ver f�r�ndras under spelets g�ng, t.ex animerade sprites
		virtual void engineUpdate(std::vector<GameObject*> objs) {} 

		GameObject(const GameObject& other) = delete; 
		GameObject& operator=(const GameObject& other) = delete; 

	protected:
		GameObject(int x, int y);
		SDL_RendererFlip getFlipType() const { return flipType; }

	private:
		SDL_Rect rect;
		float actualPosX, actualPosY; // Anv�nds s� man kan addera br�kdelar av en pixel f�r positionen (avrundas sen till hela pixlar) 
		bool active = true;
		bool stopAtWindowBorder = false; // Om ett objekt kan r�ra sig utanf�r spelsk�rmen 
		void (*updateMethodToCall) () = nullptr; 
		std::string name; 
		GameObject* parent = nullptr; 

		int rotationAngle = 0;
		SDL_RendererFlip flipType = SDL_FLIP_NONE;

		std::vector<GameObject*> children;

		inline static int objectCount = 0; 

		friend void Scene::remove(GameObject* gObj); // S� scene kan ta bort children vid remove 
	};
}

#endif