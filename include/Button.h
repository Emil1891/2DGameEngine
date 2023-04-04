#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>

#include "Sprite.h"
#include "Text.h"

namespace engine {
	class Button : public Sprite
	{
	public:

		// Dessa färger används om man inte angivit en egen bild för hur knappen ska se ut vid klick eller bild för när knappen är inaktiv 
		struct ButtonColor {
			SDL_Color enabled = { 255, 255, 255, 255 };
			SDL_Color pressed = { 175, 175, 175, 255 };
			SDL_Color disabled = { 75, 75, 75, 255 };
		};

		static Button* getInstance(int x, int y, std::string text, std::string fileName = defaultButtonImageName);

		virtual void mouseDown(const SDL_Event&); 
		virtual void mouseUp(const SDL_Event&); 

		virtual void onClick(); 
		void setOnClick(void(*onClickFunc)()) { this->onClickFunc = onClickFunc; }

		// Returnerar ett text objekt som kan användas för att redigera texten i knappen 
		Text* getText() { return text; }
		
		/// <summary> Ändrar knappens normala/default bild </summary>
		/// <param name="fileName"> Filnamnet på den nya bilden som måste vara placerad i .../images/ mappen </param>
		/// <param name="keepSize"> Behåller den gamla bildens bredd och höjd om den sätts till true </param>
		void setButtonImage(std::string fileName, bool keepSize = true); 
		
		/// <summary> Ändrar knappens bild som visas när knappen blir nedtryckt. OBS att bilden kommer stretchas om den inte är samma storlek som standardbilden, 
		/// kan alternativt sätta bredd och höjd manuellt men då blir standardbilden stretchad istället </summary>
		/// <param name="fileName"> Filnamnet på den nya bilden som måste vara placerad i .../images/ mappen </param>
		void setPressedButtonImage(std::string fileName);

		/// <summary> Ändrar knappens bild som visas om knappen är avstängd. OBS att bilden kommer stretchas om den inte är samma storlek som standardbilden, 
		/// kan alternativt sätta bredd och höjd manuellt men då blir standardbilden stretchad istället </summary>
		/// <param name="fileName"> Filnamnet på den nya bilden som måste vara placerad i .../images/ mappen </param>
		void setDisabledButtonImage(std::string fileName);

		void draw() const; 

		void setEnabled(const bool); 
		bool isEnabled() const { return enabled; }

		~Button(); 

	protected:
		Button(int x, int y, std::string text, std::string fileName = defaultButtonImageName); 

	private:
		SDL_Texture* pressedButton = nullptr, *disabledButton = nullptr, *imageToDraw = nullptr; // Texturen för default knapp utseendet är texturen i Sprite klassen (superklassen)
		Text* text;
		void (*onClickFunc) () = nullptr; 
		bool enabled = true;
		bool startedClick = false; // Håller koll på om man klickat på knappen så inte man kan klicka utanför och släppa över och det räknas som en klick

		inline static const std::string defaultButtonImageName = "button.png";
	};
}

#endif