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

		// Dessa f�rger anv�nds om man inte angivit en egen bild f�r hur knappen ska se ut vid klick eller bild f�r n�r knappen �r inaktiv 
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

		// Returnerar ett text objekt som kan anv�ndas f�r att redigera texten i knappen 
		Text* getText() { return text; }
		
		/// <summary> �ndrar knappens normala/default bild </summary>
		/// <param name="fileName"> Filnamnet p� den nya bilden som m�ste vara placerad i .../images/ mappen </param>
		/// <param name="keepSize"> Beh�ller den gamla bildens bredd och h�jd om den s�tts till true </param>
		void setButtonImage(std::string fileName, bool keepSize = true); 
		
		/// <summary> �ndrar knappens bild som visas n�r knappen blir nedtryckt. OBS att bilden kommer stretchas om den inte �r samma storlek som standardbilden, 
		/// kan alternativt s�tta bredd och h�jd manuellt men d� blir standardbilden stretchad ist�llet </summary>
		/// <param name="fileName"> Filnamnet p� den nya bilden som m�ste vara placerad i .../images/ mappen </param>
		void setPressedButtonImage(std::string fileName);

		/// <summary> �ndrar knappens bild som visas om knappen �r avst�ngd. OBS att bilden kommer stretchas om den inte �r samma storlek som standardbilden, 
		/// kan alternativt s�tta bredd och h�jd manuellt men d� blir standardbilden stretchad ist�llet </summary>
		/// <param name="fileName"> Filnamnet p� den nya bilden som m�ste vara placerad i .../images/ mappen </param>
		void setDisabledButtonImage(std::string fileName);

		void draw() const; 

		void setEnabled(const bool); 
		bool isEnabled() const { return enabled; }

		~Button(); 

	protected:
		Button(int x, int y, std::string text, std::string fileName = defaultButtonImageName); 

	private:
		SDL_Texture* pressedButton = nullptr, *disabledButton = nullptr, *imageToDraw = nullptr; // Texturen f�r default knapp utseendet �r texturen i Sprite klassen (superklassen)
		Text* text;
		void (*onClickFunc) () = nullptr; 
		bool enabled = true;
		bool startedClick = false; // H�ller koll p� om man klickat p� knappen s� inte man kan klicka utanf�r och sl�ppa �ver och det r�knas som en klick

		inline static const std::string defaultButtonImageName = "button.png";
	};
}

#endif