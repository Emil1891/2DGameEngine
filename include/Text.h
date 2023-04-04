#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Sprite.h"
#include "Constants.h"

namespace engine {
    class Text : public GameObject
    {
    public: 
        // Max width �r bredden innan raden ska brytas, anges inget s� bryts aldrig raden 
        static Text* getInstance(int x, int y, std::string text, int maxWidth = 0); 

        void draw() const; 

        /// <summary> Centrerar texten inom ett game object  </summary>
        /// <param name="obj"> Objektet som texten centreras i </param>
        /// <param name="centerHorizontally"> om den ska centrera horisontellt. Standard: true. </param>
        /// <param name="centerVertically"> om den ska centrera vertikalt. Standard: true. </param>
        void centerText(GameObject* obj, bool centerHorizontally = true, bool centerVertically = true);

        std::string getText() const { return text; }
        void setText(std::string); 

        /// <summary> �ndrar textens typsnitt </summary>
        /// <param name="fileName"> Namnet p� filen som m�ste vara placerad i ../fonts/ mappen</param>
        void setFont(std::string fileName); 

        void setFontSize(int size); 
        int getFontSize() const { return fontSize; }

        // �ndrar f�rgen p� texten d�r den nya f�rgen tas emot som ett RGB-v�rde 
        void setColor(int r, int g, int b, int a = 255); 
        // �ndrar f�rgen p� texten d�r den nya f�rgen tas emot som ett objekt av SDL_Color
        void setColor(SDL_Color& color); 

        // �ndrar det horisontella avst�ndet mellan texten och objektet som texten �r centrerat i. �r den ej centrerad sker inget. 
        // Flytta textens position om st�rre avst�nd �nskas men texten inte ska vara centrerad i n�t. 
        void setHorizontalPadding(int padding); 
        int getHorizontalPadding() const { return horizontalPadding; }

        /// <summary> �ndrar l�ngden i pixlar tills raden bryts automatiskt, 0 betyder att den aldrig bryts exkl. new line karakt�rer. 
        /// Anv�nd denna funktion p� text objekt ist�llet f�r setW() om du vill undvika stretching </summary>
        /// <param name="width"> Antal pixlar till radbryt </param>
        void setMaxWidth(int width);
        
        /// <summary> Visar n�r raden bryts automatiskt, allts� exkl. new line karakt�rer </summary>
        /// <returns> Returnerar antalet pixlar tills raden bryter </returns>
        int getMaxWidth() const { return maxWidth; }

        ~Text(); 

    protected:
        Text(int x, int y, std::string text, int maxWidth = 0);

    private: 
        std::string text; 
        int fontSize; 
        TTF_Font* font; 
        std::string fontPath = constants::gResPath + "fonts/arial.ttf";

        SDL_Texture* texture = nullptr; 
        int maxWidth; 
        SDL_Color color = { 0, 0, 0, 255 }; // Svart
        int horizontalPadding = 0; // Avst�ndet mellan texten och objektet (om n�got) texten �r kopplad till  
        GameObject* objectConnectedTo = nullptr; 

        bool centeredHori = false, centeredVert = false; 

        // Skapar en ny textur, �ndra objektets v�rden s�som f�rg, font osv innan denna kallas 
        void updateTexture(); 
    };
}

#endif