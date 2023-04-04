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
        // Max width är bredden innan raden ska brytas, anges inget så bryts aldrig raden 
        static Text* getInstance(int x, int y, std::string text, int maxWidth = 0); 

        void draw() const; 

        /// <summary> Centrerar texten inom ett game object  </summary>
        /// <param name="obj"> Objektet som texten centreras i </param>
        /// <param name="centerHorizontally"> om den ska centrera horisontellt. Standard: true. </param>
        /// <param name="centerVertically"> om den ska centrera vertikalt. Standard: true. </param>
        void centerText(GameObject* obj, bool centerHorizontally = true, bool centerVertically = true);

        std::string getText() const { return text; }
        void setText(std::string); 

        /// <summary> Ändrar textens typsnitt </summary>
        /// <param name="fileName"> Namnet på filen som måste vara placerad i ../fonts/ mappen</param>
        void setFont(std::string fileName); 

        void setFontSize(int size); 
        int getFontSize() const { return fontSize; }

        // Ändrar färgen på texten där den nya färgen tas emot som ett RGB-värde 
        void setColor(int r, int g, int b, int a = 255); 
        // Ändrar färgen på texten där den nya färgen tas emot som ett objekt av SDL_Color
        void setColor(SDL_Color& color); 

        // Ändrar det horisontella avståndet mellan texten och objektet som texten är centrerat i. Är den ej centrerad sker inget. 
        // Flytta textens position om större avstånd önskas men texten inte ska vara centrerad i nåt. 
        void setHorizontalPadding(int padding); 
        int getHorizontalPadding() const { return horizontalPadding; }

        /// <summary> Ändrar längden i pixlar tills raden bryts automatiskt, 0 betyder att den aldrig bryts exkl. new line karaktärer. 
        /// Använd denna funktion på text objekt istället för setW() om du vill undvika stretching </summary>
        /// <param name="width"> Antal pixlar till radbryt </param>
        void setMaxWidth(int width);
        
        /// <summary> Visar när raden bryts automatiskt, alltså exkl. new line karaktärer </summary>
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
        int horizontalPadding = 0; // Avståndet mellan texten och objektet (om något) texten är kopplad till  
        GameObject* objectConnectedTo = nullptr; 

        bool centeredHori = false, centeredVert = false; 

        // Skapar en ny textur, ändra objektets värden såsom färg, font osv innan denna kallas 
        void updateTexture(); 
    };
}

#endif