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

		// OBS att det avrundas till hela pixlar vid visning på skärmen 
		void setX(float x);

		// OBS att det avrundas till hela pixlar vid visning på skärmen 
		void setY(float y);

		// OBS att det avrundas till hela pixlar vid visning på skärmen 
		void setPos(float x, float y) { setX(x); setY(y); }
		void setPos(Vector vec) { setPos(vec.x, vec.y); }

		// OBS att pixel kollisioner inte tar hänsyn till ändrade storlekar. De sker alltså på originalstorleken. Ändra storlek i photoshop om pixel kollisioner önskas. 
		// Rect koll. (åt båda hållen) fungerar bra. 
		void setW(int w);
		// OBS att pixel kollisioner inte tar hänsyn till ändrade storlekar. De sker alltså på originalstorleken. Ändra storlek i photoshop om pixel kollisioner önskas. 
		// Rect koll. (åt båda hållen) fungerar bra. 
		void setH(int h);
		virtual void setSize(int w, int h) { setW(w); setH(h); } // Virtual eftersom den överskuggas i particlesystem 

		/// <summary> Lägger ett objekt som dess child som följer med sin parent om den flyttas. Alla children till ett objekt tas också bort om parent:en förstörs </summary>
		/// <param name="obj"> Objektet som ska bli en child </param>
		void addChild(GameObject* obj); 

		/// <summary> Tar bort ett objekt som en child. </summary>
		/// <param name="obj"> Objektet som ska tas bort som child </param>
		/// <returns> Returnerar pekare till objektet som togs bort om det lyckades eller nullptr om det inte hittades. </returns>
		GameObject* removeChild(GameObject* obj);

		bool hasChildren() const { return !children.empty(); }

		bool isActive() const { return active; };
		/// <summary> Om objektet är inaktivt så kommer det inte att renderas alls. Ev. children sätts också inaktiva/aktiva. </summary>
		/// <param name="toggle"> Aktiv true/false </param>
		void setActive(const bool toggle);

		SDL_Rect getRect() const { return rect; }

		std::string getName() const { return name; }
		void setName(std::string name) { this->name = name; }

		// Kallas på varje frame och ritar ut objektet 
		virtual void draw() const = 0;

		// Kallas på varje frame
		virtual void update();

		/// <summary> Sätter funktionen som körs i update(), dvs varje frame. Funktionen kan skrivas direkt i en överskuggad update 
		/// om man skapar en ny klass som ärver från GameObject eller dess subklasser </summary>
		/// <param name="newFunction"> Tar emot en funktionspekare som returnerar void och inte tar några argument </param>
		void setOnUpdate(void (*newFunction) ()) { updateMethodToCall = newFunction; }

		/// <summary> Roterar bilden. OBS att rektangeln som bestämmer bl.a klickyta INTE uppdateras och att den alltid är en rektangel, så rotationer (exkl. 180*n) gör att kollisioner/klick 
		/// kan ske utanför bilden. För knappar och andra interagerbara föremål bör rotationen alltså ske i t.ex. photoshop innan den läggs in i spelet. </summary>
		/// <param name="rotation"> i antal grader. </param>
		void setRotation(int rotation); 
		int getRotation() const { return rotationAngle; }

		/// <summary> För att flippa bilden längs x-axeln. Om redan flippad så flippar den tillbaka. För att återställa helt kan resetFlip() användas. </summary>
		void flipX();
		/// <summary> För att flippa bilden längs x-axeln. Om redan flippad så flippar den tillbaka. För att återställa helt kan resetFlip() användas. </summary>
		void flipY(); 
		/// <summary> Återställer bildens flip på båda axlarna. </summary>
		void resetFlip();

		/// <summary> Sätter om det ska vara möjligt att röra sig utanför fönstret. Inaktivt från början. Om aktivt görs det en check vid förflyttning (setX(), setY(), setPos(). </summary>
		/// <param name="toggle"> true = stoppar figuren från att röra sig utanför. </param>
		void setStopAtWindowBorder(const bool toggle) { stopAtWindowBorder = toggle; }
		bool getStopAtWindowBorder() const { return stopAtWindowBorder; }

		virtual ~GameObject();

		// Används bara av spelmotorn för objekt som behöver förändras under spelets gång, t.ex animerade sprites
		virtual void engineUpdate(std::vector<GameObject*> objs) {} 

		GameObject(const GameObject& other) = delete; 
		GameObject& operator=(const GameObject& other) = delete; 

	protected:
		GameObject(int x, int y);
		SDL_RendererFlip getFlipType() const { return flipType; }

	private:
		SDL_Rect rect;
		float actualPosX, actualPosY; // Används så man kan addera bråkdelar av en pixel för positionen (avrundas sen till hela pixlar) 
		bool active = true;
		bool stopAtWindowBorder = false; // Om ett objekt kan röra sig utanför spelskärmen 
		void (*updateMethodToCall) () = nullptr; 
		std::string name; 
		GameObject* parent = nullptr; 

		int rotationAngle = 0;
		SDL_RendererFlip flipType = SDL_FLIP_NONE;

		std::vector<GameObject*> children;

		inline static int objectCount = 0; 

		friend void Scene::remove(GameObject* gObj); // Så scene kan ta bort children vid remove 
	};
}

#endif