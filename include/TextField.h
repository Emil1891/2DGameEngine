#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "Sprite.h"
#include "Text.h"

#include <string>

namespace engine {
    class TextField : public Sprite
    {
    public: 
        /// <summary> Skapar ett textf�lt som kan anv�ndas f�r inmatning av text fr�n anv�ndaren. </summary>
        /// <param name="x"> koordinat </param>
        /// <param name="y"> koordinat </param>
        /// <param name="placeholderText"> texten som visas n�r anv�ndaren inte skrivit n�t �n. </param>
        /// <returns> returnerar en pekare till att textfield objekt som kan l�ggas till i �nskad scen. </returns>
        static TextField* getInstance(int x, int y, std::string placeholderText);

        virtual void mouseDown(const SDL_Event&); 

        void engineUpdate(std::vector<GameObject*> objs) override;

        void draw() const override; 

        void write(SDL_Event& eve); // Skriva bokst�ver 

        void edit(SDL_Event& eve); // Redigera (copy, paste, backspace) 

        /// <summary> Om man vill f�rhindra input fast fortfarande rendera f�ltet. </summary>
        /// <param name="toggle"> p� eller av. </param>
        void setEnabled(const bool toggle) { enabled = toggle; }

        /// <summary> Kan anv�ndas f�r att ta reda p� om anv�ndaren h�ller p� skriva i textf�ltet. </summary>
        /// <returns> returnerar om textf�ltet �r i fokus, dvs anv�ndaren har klickat i det. </returns>
        bool isFocused() const { return inFocus; }

        /// <summary> Returnerar text objektet som visas n�r anv�ndaren inte skrivit n�t. Kan redigeras som ett vanligt textobjekt. </summary>
        Text* getPlaceholderText() const { return placeholderText; }

        /// <summary> Returnerar text objektet som inneh�ller texten anv�ndaren skrivit. Kan redigeras som ett vanligt textobjekt. </summary>
        Text* getInputText() const { return inputText; }

        ~TextField(); 

    protected:
        TextField(int x, int y, std::string placeholderText); 

    private:
        SDL_Texture* inputField = getTexture(); // Denna pekar till texturen i sprite superklassen 
        Sprite* textMarker; 
        Text* placeholderText; 
        Text* inputText; 

        bool enabled = true;
        bool inFocus = false; 

        inline static const std::string DEFAULT_IMAGE = "textfield.png"; 

        int currentTextIndex = 0; 

        void unFocus(); 

        // Add
        void addText(SDL_Event& eve); 
        void insertText(SDL_Event& eve); 

        // Edit
        void deleteBackspace(); 
        void pasteText(); 
        void moveMarkerLeft();
        void moveMarkerRight();

        void updateMarkerPosAndPlaceholderText(); 
    };
}

#endif