#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>
#include <unordered_set>

#include "GameObject.h"
#include "Session.h"
#include "SpriteComponent.h"

namespace engine {
	class Sprite : public GameObject
	{
	public:
		
		/// <summary> Funktionen som kallas p� f�r att skapa Sprite objekt </summary>
		/// <param name="x"> X-koordinaten</param> <param name="y"> Y-koordinaten</param>
		/// <param name="fileName"> Namnet p� filen som m�ste vara placerad i .../resources/images/ mappen</param>
		/// <returns> Returnerar en pekare till ett Sprite objekt </returns>
		static Sprite* getInstance(int x, int y, std::string fileName);

		void draw() const;

		SDL_Texture* getTexture() { return texture; }

		/// <summary> �ndrar bilden som ska renderas, anv�nd ist�llet setButtonImage() om du vill �ndra en knapps standardbild </summary>
		/// <param name="fileName"> Namnet p� filen som m�ste vara placerad i .../resources/images/ mappen </param>
		/// <param name="keepWidthAndheight"> Beh�ller den gamla bildens bredd och h�jd om den s�tts till true </param>
		void changeImage(std::string fileName, bool keepWidthAndheight = false); 

		/// <summary> �ndrar f�rgen p� texturen d�r f�rgen mulitpliceras p� bildens ursprungliga f�rg och full vit (255, 255, 255) g�r att den �r of�r�ndrad. 
		///  Tal utanf�r 0-255 clamp:as automatiskt. </summary>
		/// <param name="r"> Red </param> <param name="g"> Green </param> <param name="b"> Blue </param> <param name="a"> Alpha </param>
		void setColor(int r, int g, int b, int a = 255); 
		
		/// <summary> �ndrar f�rgen p� texturen d�r f�rgen mulitpliceras p� bildens ursprungliga f�rg och full vit g�r att den �r of�r�ndrad </summary>
		/// <param name="color"> Ett objekt av SDL_Color. Skulle n�gon f�rg vara utanf�r 0-255 clamp:as det automatiskt. </param>
		void setColor(SDL_Color& color); 

		/// <summary> �ndrar enbart alpha v�rdet. Anv�nd setColor(...) f�r att byta f�rg. </summary>
		/// <param name="a"> det nya alpha v�rdet (0-255). 0 = osynlig. Tal utanf�r clamp:as automatiskt. </param>
		void setColorAlpha(int a);

		SDL_Color getColor() const { return color; }

		void engineUpdate(std::vector<GameObject*> objs) override;

		/// <summary> Funktionen som kallas vid kollision. Kan antingen �verskuggas eller ange funktion som ska kallas med setOnCollisionEnter(...). 
		/// OBS att b�da objekten i en kollision m�ste ha collisions aktiverade med toggleCollisions(true). </summary>
		/// <param name="collision"> Objektet som kolliderades med. </param>
		virtual void onCollisionEnter(GameObject* collision) { if (onCollisionEnterFunc != nullptr) onCollisionEnterFunc(collision); }

		/// <summary> S�tter vilken funktion som anropas vid kollision. OBS att b�da objekten i en kollision m�ste ha kollision aktiverade. Anv�nd inte denna om onCollisionEnter �verskuggas. </summary>
		/// <param name="onCollide"> Objektet som kolliderades med. </param>
		void setOnCollisionEnter(void (*onCollisionEnterFunc)(GameObject* collision)) { this->onCollisionEnterFunc = onCollisionEnterFunc; }

		/// <summary> Kallas n�r tv� objekt slutar kollidera med varandra. �verskugga eller ange funktion med setOnCollisionExit(...). </summary>
		/// <param name="collision"> Objektet som slutar kollidera. </param>
		virtual void onCollisionExit(GameObject* collision) { if (onCollisionExitFunc != nullptr) onCollisionExitFunc(collision); }

		/// <summary> S�tter vilken funktion som anropas n�r tv� objekt slutar kollidera. Anv�nd inte denna om onCollisionExit �verskuggas. </summary>
		/// <param name="onCollide"> Objektet som slutade kolliderades med. </param>
		void setOnCollisionExit(void (*onCollisionExitFunc)(GameObject* collision)) { this->onCollisionExitFunc = onCollisionExitFunc; }

		/// <summary> Anropas varje frame som objekten kolliderar med varandra. �verskugga eller s�tt funktion som anropas med setonCollisionStay(...) </summary>
		/// <param name="collision"> Objektet som kollideras med. </param>
		virtual void onCollisionStay(GameObject* collision) { if (onCollisionStayFunc != nullptr) onCollisionStayFunc(collision); }

		/// <summary> S�tter vilken funktion som anropas varje frame som tv� objekt kolliderar. Anv�nd inte denna om onCollisionStay �verskuggas. </summary>
		/// <param name="onCollide"> Objektet som kollideras med. </param>
		void setOnCollisionStay(void (*onCollisionStayFunc)(GameObject* collision)) { this->onCollisionStayFunc = onCollisionStayFunc; }

		/// <summary> Kollar om objektet kolliderar denna frame med vadsomhelst (som ocks� har kollisioner aktiverat). </summary>
		/// <returns> true om kolliderar. </returns>
		bool isColliding() const { return colliding; }

		// L�gger till en komponent p� ditt objekt med "mallsyntax". T.ex: mySprite->addComponent<Collider>(); 
		// Returnerar pekare till komponenten som skapats. Fanns redan komponenten returneras pekaren till den. 
		template<std::derived_from<SpriteComponent> T> // Motsvarighet till javas <T extends Class> 
		T* addComponent();

		// Anv�nds f�r att f� tag p� en viss komponent enligt: sprite->getComponent<Typ>(); 
		// Returnerar komponenten eller 0 om den inte fanns. </returns>
		template<std::derived_from<SpriteComponent> T>
		T* getComponent();

		template<std::derived_from<SpriteComponent> T>
		T* removeComponent();

		~Sprite();

	protected:
		Sprite(int x, int y, std::string fileName);

	private:
		SDL_Color color = { 255, 255, 255, 255 }; 
		SDL_Texture* texture = 0;
		SDL_Surface* surface = 0;

		void (*onCollisionEnterFunc)(GameObject* collision) = nullptr; 
		void (*onCollisionExitFunc)(GameObject* collision) = nullptr;
		void (*onCollisionStayFunc)(GameObject* collision) = nullptr;
		bool colliding = false; // Om objektet kolliderar just nu
		
		std::vector<SpriteComponent*> components; 

		friend class Collider; 
		friend class Rigidbody; 
	};

	template<std::derived_from<SpriteComponent> T>
	T* Sprite::addComponent()
	{
		for (auto c : components) // Till�ter inte duplicates 
			if (T* t = dynamic_cast<T*>(c))
				return t; 
		T* comp = new T(this);
		components.push_back(comp);
		return comp; 
	}

	template<std::derived_from<SpriteComponent> T>
	T* Sprite::getComponent()
	{
		for (auto c : components) 
			if (T* t = dynamic_cast<T*>(c)) 
				return t;
		return 0;
	}

	template<std::derived_from<SpriteComponent> T>
	T* Sprite::removeComponent() 
	{
		auto it = components.begin(); 
		while (it != components.end()) {
			if (T* t = dynamic_cast<T*>(*it)) { // Om klassen/komponenten finns
				components.erase(it);
				return t; 
			}
			++it; 
		}
		return 0;
	}
}

#endif