#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "Sprite.h"
#include "Text.h"

#include <string>

namespace engine {
    class TextField : public Sprite
    {
    public: 
        /// <summary> Skapar ett textfält som kan användas för inmatning av text från användaren. </summary>
        /// <param name="x"> koordinat </param>
        /// <param name="y"> koordinat </param>
        /// <param name="placeholderText"> texten som visas när användaren inte skrivit nåt än. </param>
        /// <returns> returnerar en pekare till att textfield objekt som kan läggas till i önskad scen. </returns>
        static TextField* getInstance(int x, int y, std::string placeholderText);

        virtual void mouseDown(const SDL_Event&); 

        void engineUpdate(std::vector<GameObject*> objs) override;

        void draw() const override; 

        void write(SDL_Event& eve); // Skriva bokstäver 

        void edit(SDL_Event& eve); // Redigera (copy, paste, backspace) 

        /// <summary> Om man vill förhindra input fast fortfarande rendera fältet. </summary>
        /// <param name="toggle"> på eller av. </param>
        void setEnabled(const bool toggle) { enabled = toggle; }

        /// <summary> Kan användas för att ta reda på om användaren håller på skriva i textfältet. </summary>
        /// <returns> returnerar om textfältet är i fokus, dvs användaren har klickat i det. </returns>
        bool isFocused() const { return inFocus; }

        /// <summary> Returnerar text objektet som visas när användaren inte skrivit nåt. Kan redigeras som ett vanligt textobjekt. </summary>
        Text* getPlaceholderText() const { return placeholderText; }

        /// <summary> Returnerar text objektet som innehåller texten användaren skrivit. Kan redigeras som ett vanligt textobjekt. </summary>
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