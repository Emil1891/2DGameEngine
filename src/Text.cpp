#include "Text.h"
#include "System.h"
#include "Constants.h"

#include <algorithm>

namespace engine {

	Text* engine::Text::getInstance(int x, int y, std::string text, int maxWidth)
	{
		return new Text(x, y, text, maxWidth); 
	}

	void Text::updateTexture()
	{
		if(texture != nullptr) // Första gången denna funktion kallas så kommer det inte finnas en textur
			SDL_DestroyTexture(texture); 
		SDL_Surface* surf = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, maxWidth - horizontalPadding);
		texture = SDL_CreateTextureFromSurface(sys.getRen(), surf);
		SDL_FreeSurface(surf);
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		setW(w);
		setH(h);
		if (objectConnectedTo != nullptr)
			centerText(objectConnectedTo, centeredHori, centeredVert);
	}

	Text::Text(int x, int y, std::string text, int maxWidth) : GameObject(x, y), text(text), fontSize(sys.getStdFontSize()), maxWidth(maxWidth), font(sys.getFont())
	{
		updateTexture(); 
	}

	void Text::draw() const
	{
		const SDL_Rect& rect = getRect();
		SDL_RenderCopyEx(sys.getRen(), texture, NULL, &rect, getRotation(), NULL, getFlipType());
	}

	void Text::centerText(GameObject* obj, bool centerHorizontally, bool centerVertically)
	{
		centeredHori = centerHorizontally;
		centeredVert = centerVertically; 
		if (centerHorizontally) {
			int midObjX = obj->getX() + horizontalPadding + (obj->getW() / 2) - horizontalPadding - getW() / 2;
			setX(midObjX);
		}
		else
			setX(obj->getX() + horizontalPadding); 

		if (centerVertically) {
			int midObjY = obj->getY() + (obj->getH() / 2) - getH() / 2;
			setY(midObjY);
		}
		objectConnectedTo = obj; 
	}

	void Text::setText(std::string newText)
	{
		text = newText;
		updateTexture(); 
	}

	void Text::setFont(std::string fileName)
	{
		if (font != sys.getFont())
			TTF_CloseFont(font);
		std::string prevFontPath = fontPath; 
		fontPath = constants::gResPath + "fonts/" + fileName;
		font = TTF_OpenFont(fontPath.c_str(), fontSize);
		if (font == NULL) {
			std::cout << "Kunde inte ladda in typsnitt " + fontPath << " Anvander senaste istallet." << std::endl;
			fontPath = prevFontPath; 
			font = TTF_OpenFont(fontPath.c_str(), fontSize); 
		}
		updateTexture(); 
	}

	void Text::setFontSize(int size)
	{
		fontSize = size;
		if (font != sys.getFont())
			TTF_CloseFont(font);
		font = TTF_OpenFont(fontPath.c_str(), size);
		updateTexture(); 
	}

	void Text::setColor(int r, int g, int b, int a)
	{
		r = std::clamp(r, 0, 255);
		g = std::clamp(g, 0, 255);
		b = std::clamp(b, 0, 255);
		a = std::clamp(a, 0, 255);
		color.r = r; 
		color.g = g; 
		color.b = b; 
		color.a = a; 
		updateTexture();
	}

	void Text::setColor(SDL_Color& color)
	{
		setColor(color.r, color.g, color.b, color.a); 
	}

	void Text::setHorizontalPadding(int padding)
	{
		if (objectConnectedTo == nullptr)
			return; 
		this->horizontalPadding = padding; 
		updateTexture(); 
	}

	void Text::setMaxWidth(int width)
	{
		maxWidth = width; 
		updateTexture(); 
	}

	Text::~Text()
	{
		SDL_DestroyTexture(texture); 
		if(font != sys.getFont())
			TTF_CloseFont(font); 
	}
}