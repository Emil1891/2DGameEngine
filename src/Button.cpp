#include <string>
#include <SDL2/SDL_image.h>

#include "Constants.h"
#include "Button.h"
#include "System.h"

namespace engine {

	struct Button::ButtonColor buttonColor;

	Button* Button::getInstance(int x, int y, std::string text, std::string fileName)
	{
		return new Button(x, y, text, fileName);
	}

	Button::Button(int x, int y, std::string str, std::string fileName) : Sprite(x, y, fileName), imageToDraw(getTexture()), text(Text::getInstance(x, y, str, getW()))
	{
		addChild(text);
		text->centerText(this);
		text->setHorizontalPadding(15); 
	}

	void Button::mouseDown(const SDL_Event& eve)
	{
		if (!enabled || eve.button.button != SDL_BUTTON_LEFT || !isActive()) // Om knappen �r avst�ngd eller man inte klickar med v�nster musknapp: return
			return; 
		SDL_Point p = { eve.button.x, eve.button.y };
		SDL_Rect rect = getRect(); 
		if (SDL_PointInRect(&p, &rect)) {
			if (pressedButton == nullptr) // Om man inte angivit en bild som ska anv�ndas vid klick 
				setColor(buttonColor.pressed);
			else
				imageToDraw = pressedButton; 
			startedClick = true;
		}
	}

	void Button::mouseUp(const SDL_Event& eve)
	{
		if (!enabled || eve.button.button != SDL_BUTTON_LEFT || !startedClick)
			return; 
		SDL_Point p = { eve.button.x, eve.button.y };
		SDL_Rect rect = getRect();
		if (SDL_PointInRect(&p, &rect))
			onClick(); 
		if (enabled) { 
			setColor(buttonColor.enabled);
			imageToDraw = getTexture();
		}
		startedClick = false; 
	}

	void Button::onClick()
	{
		if (onClickFunc != nullptr)
			onClickFunc(); 
	}

	void Button::setButtonImage(std::string fileName, bool keepSize)
	{
		changeImage(fileName, keepSize); // Kallar p� funktionen i Sprite eftersom default knappen anv�nder sig av texturen som �rvs d�rifr�n 
		imageToDraw = getTexture();
	}

	void Button::setPressedButtonImage(std::string fileName)
	{
		SDL_DestroyTexture(pressedButton);
		pressedButton = IMG_LoadTexture(sys.getRen(), (constants::gResPath + "/images/" + fileName).c_str());
	}

	void Button::setDisabledButtonImage(std::string fileName)
	{
		SDL_DestroyTexture(disabledButton);
		disabledButton = IMG_LoadTexture(sys.getRen(), (constants::gResPath + "/images/" + fileName).c_str());
	}

	void Button::draw() const
	{
		const SDL_Rect& rect = getRect();
		SDL_RenderCopyEx(sys.getRen(), imageToDraw, NULL, &rect, getRotation(), NULL, getFlipType());
		text->draw(); 
	}

	void Button::setEnabled(const bool toggle) {
		if (disabledButton == nullptr)
			setColor(toggle == true ? buttonColor.enabled : buttonColor.disabled);
		else
			imageToDraw = toggle == true ? getTexture() : disabledButton;
		enabled = toggle; 
	}

	Button::~Button()
	{
		SDL_DestroyTexture(pressedButton);
		SDL_DestroyTexture(disabledButton); 
		SDL_DestroyTexture(imageToDraw); 
	}
}