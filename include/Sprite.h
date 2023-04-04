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
		
		/// <summary> Funktionen som kallas på för att skapa Sprite objekt </summary>
		/// <param name="x"> X-koordinaten</param> <param name="y"> Y-koordinaten</param>
		/// <param name="fileName"> Namnet på filen som måste vara placerad i .../resources/images/ mappen</param>
		/// <returns> Returnerar en pekare till ett Sprite objekt </returns>
		static Sprite* getInstance(int x, int y, std::string fileName);

		void draw() const;

		SDL_Texture* getTexture() { return texture; }

		/// <summary> Ändrar bilden som ska renderas, använd istället setButtonImage() om du vill ändra en knapps standardbild </summary>
		/// <param name="fileName"> Namnet på filen som måste vara placerad i .../resources/images/ mappen </param>
		/// <param name="keepWidthAndheight"> Behåller den gamla bildens bredd och höjd om den sätts till true </param>
		void changeImage(std::string fileName, bool keepWidthAndheight = false); 

		/// <summary> Ändrar färgen på texturen där färgen mulitpliceras på bildens ursprungliga färg och full vit (255, 255, 255) gör att den är oförändrad. 
		///  Tal utanför 0-255 clamp:as automatiskt. </summary>
		/// <param name="r"> Red </param> <param name="g"> Green </param> <param name="b"> Blue </param> <param name="a"> Alpha </param>
		void setColor(int r, int g, int b, int a = 255); 
		
		/// <summary> Ändrar färgen på texturen där färgen mulitpliceras på bildens ursprungliga färg och full vit gör att den är oförändrad </summary>
		/// <param name="color"> Ett objekt av SDL_Color. Skulle någon färg vara utanför 0-255 clamp:as det automatiskt. </param>
		void setColor(SDL_Color& color); 

		/// <summary> Ändrar enbart alpha värdet. Använd setColor(...) för att byta färg. </summary>
		/// <param name="a"> det nya alpha värdet (0-255). 0 = osynlig. Tal utanför clamp:as automatiskt. </param>
		void setColorAlpha(int a);

		SDL_Color getColor() const { return color; }

		void engineUpdate(std::vector<GameObject*> objs) override;

		/// <summary> Funktionen som kallas vid kollision. Kan antingen överskuggas eller ange funktion som ska kallas med setOnCollisionEnter(...). 
		/// OBS att båda objekten i en kollision måste ha collisions aktiverade med toggleCollisions(true). </summary>
		/// <param name="collision"> Objektet som kolliderades med. </param>
		virtual void onCollisionEnter(GameObject* collision) { if (onCollisionEnterFunc != nullptr) onCollisionEnterFunc(collision); }

		/// <summary> Sätter vilken funktion som anropas vid kollision. OBS att båda objekten i en kollision måste ha kollision aktiverade. Använd inte denna om onCollisionEnter överskuggas. </summary>
		/// <param name="onCollide"> Objektet som kolliderades med. </param>
		void setOnCollisionEnter(void (*onCollisionEnterFunc)(GameObject* collision)) { this->onCollisionEnterFunc = onCollisionEnterFunc; }

		/// <summary> Kallas när två objekt slutar kollidera med varandra. Överskugga eller ange funktion med setOnCollisionExit(...). </summary>
		/// <param name="collision"> Objektet som slutar kollidera. </param>
		virtual void onCollisionExit(GameObject* collision) { if (onCollisionExitFunc != nullptr) onCollisionExitFunc(collision); }

		/// <summary> Sätter vilken funktion som anropas när två objekt slutar kollidera. Använd inte denna om onCollisionExit överskuggas. </summary>
		/// <param name="onCollide"> Objektet som slutade kolliderades med. </param>
		void setOnCollisionExit(void (*onCollisionExitFunc)(GameObject* collision)) { this->onCollisionExitFunc = onCollisionExitFunc; }

		/// <summary> Anropas varje frame som objekten kolliderar med varandra. Överskugga eller sätt funktion som anropas med setonCollisionStay(...) </summary>
		/// <param name="collision"> Objektet som kollideras med. </param>
		virtual void onCollisionStay(GameObject* collision) { if (onCollisionStayFunc != nullptr) onCollisionStayFunc(collision); }

		/// <summary> Sätter vilken funktion som anropas varje frame som två objekt kolliderar. Använd inte denna om onCollisionStay överskuggas. </summary>
		/// <param name="onCollide"> Objektet som kollideras med. </param>
		void setOnCollisionStay(void (*onCollisionStayFunc)(GameObject* collision)) { this->onCollisionStayFunc = onCollisionStayFunc; }

		/// <summary> Kollar om objektet kolliderar denna frame med vadsomhelst (som också har kollisioner aktiverat). </summary>
		/// <returns> true om kolliderar. </returns>
		bool isColliding() const { return colliding; }

		// Lägger till en komponent på ditt objekt med "mallsyntax". T.ex: mySprite->addComponent<Collider>(); 
		// Returnerar pekare till komponenten som skapats. Fanns redan komponenten returneras pekaren till den. 
		template<std::derived_from<SpriteComponent> T> // Motsvarighet till javas <T extends Class> 
		T* addComponent();

		// Används för att få tag på en viss komponent enligt: sprite->getComponent<Typ>(); 
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
		for (auto c : components) // Tillåter inte duplicates 
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