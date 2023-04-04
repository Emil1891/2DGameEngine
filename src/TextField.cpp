#include "TextField.h"
#include "System.h"
#include "Input.h"
#include "Sprite.h"

#include <string>

namespace { // Helper funcs.
	enum {
		SWE_O = 51, SWE_AE = 52, SWE_A = 47 
	};

	bool isSwedishLetter(SDL_Keycode& key) { // P� engelskt tangentbord �r det t.ex semikolon (vars enum har nummer 51 i SDL_SCANCODE) d�r v�rt � �r 
		return (SDL_GetScancodeFromKey(key) == SWE_A || SDL_GetScancodeFromKey(key) == SWE_AE || SDL_GetScancodeFromKey(key) == SWE_O); //  � = 47, � = 52, � = 51
	}

	char getEnglishLetter(SDL_Keycode& key) {
		if (engine::Input::getKeyDown(SDL_SCANCODE_LSHIFT) || engine::Input::getKeyDown(SDL_SCANCODE_RSHIFT)) { // Shift 
			if (SDL_GetScancodeFromKey(key) == 51) // Om �
				return 'O';
			else // M�ste vara � eller �
				return 'A';
		}
		else { // Inte shift 
			if (SDL_GetScancodeFromKey(key) == 51) // Om �
				return 'o';
			else // M�ste vara � eller �
				return 'a';
		}
	}
}

namespace engine {

	Uint32 lastUpdate = 0;

	TextField::TextField(int x, int y, std::string placeholderText) : Sprite(x, y, DEFAULT_IMAGE), placeholderText(Text::getInstance(x, y, placeholderText, getW())),
		inputText(Text::getInstance(x, y, "", getW())), textMarker(Sprite::getInstance(x + 5, y, "textmarker.png"))
	{
		textMarker->setSize(textMarker->getW() - 1, getH() - 15);
		textMarker->setY(getY() + (getH() / 2) - textMarker->getH() / 2);

		this->placeholderText->centerText(this, false, true);
		this->placeholderText->setHorizontalPadding(5);
		this->placeholderText->setColor(50, 50, 50, 150); 

		inputText->centerText(this, false, true);
		inputText->setHorizontalPadding(5);

		addChild(inputText);
		addChild(this->placeholderText);
		addChild(textMarker);
	}

	TextField* TextField::getInstance(int x, int y, std::string placeholderText)
	{
		return new TextField(x, y, placeholderText);
	}

	void TextField::mouseDown(const SDL_Event& eve)
	{
		if (!enabled || eve.button.button != SDL_BUTTON_LEFT || !isActive()) // Om knappen �r avst�ngd eller man inte klickar med v�nster musknapp: return
			return;
		SDL_Point p = { eve.button.x, eve.button.y };
		SDL_Rect rect = getRect();
		if (SDL_PointInRect(&p, &rect)) { // Klickat p� textf�ltet 
			inFocus = true;
			textMarker->setActive(true); 
			const SDL_Rect& rect = getRect();
			SDL_StartTextInput(); 
		}
		else  // Klickat n�gon annanstans
			unFocus();
		updateMarkerPosAndPlaceholderText(); 
	}

	void TextField::engineUpdate(std::vector<GameObject*> objs)
	{
		Sprite::engineUpdate(objs); 
		inputField = getTexture();
		if (!inFocus)
			return;

		textMarker->setW(1); // Om man �ndrar storlek p� saker ska inte mark�rens bredd ocks� �ndras vilket den g�r ty den �r child, detta f�rhindrar det 

		if (Input::getKey(SDL_SCANCODE_RETURN) || Input::getKey(SDL_SCANCODE_KP_ENTER)) 
			unFocus(); 

		int toggleTime = 500; // I ms
		if (SDL_GetTicks() - lastUpdate < toggleTime) // Ej tillr�cklig tid passerat 
			return;
		textMarker->setActive(!textMarker->isActive());
		lastUpdate = SDL_GetTicks();
	}

	void TextField::unFocus() {
		inFocus = false;
		textMarker->setActive(false);
		currentTextIndex = inputText->getText().length() < 0 ? 0 : inputText->getText().length(); // �terst�ller index till l�ngst fram
		SDL_StopTextInput(); 
	}

	void TextField::draw() const
	{
		const SDL_Rect& rect = getRect();
		SDL_RenderCopyEx(sys.getRen(), inputField, NULL, &rect, getRotation(), NULL, getFlipType()); // F�ltet (main-bilden)
		if (inFocus && textMarker->isActive()) 
			textMarker->draw(); // Mark�ren 

		if (inputText->getText().empty()) // Texten 
			placeholderText->draw(); 
		else
			inputText->draw(); 
	}

	void TextField::write(SDL_Event& eve)
	{
		if (!inFocus)
			return;

		if (eve.type == SDL_TEXTINPUT)
		{
			if (!(SDL_GetModState() & KMOD_CTRL && (eve.text.text[0] == 'c' || eve.text.text[0] == 'C' || eve.text.text[0] == 'v' || eve.text.text[0] == 'V'))) // Not copy or pasting
			{
				std::string oldText = inputText->getText(); 

				if (currentTextIndex == inputText->getText().length() || (currentTextIndex == 0 && inputText->getText().empty()))  // Om man har mark�ren i slutet 
					addText(eve); 
				else  // Flyttat p� den med piltangenterna 
					insertText(eve); 
				++currentTextIndex; 

				if (inputText->getW() > getW() - inputText->getHorizontalPadding() - 5) { // Slut p� f�ltet (best�mt att man inte f�r skriva mer d�) 
					inputText->setText(oldText);
					--currentTextIndex;
				}
				textMarker->setActive(true); 
				lastUpdate = SDL_GetTicks(); // S� mark�ren inte togglar p� och av medan man skriver (alltid p�)
			}
		}
		updateMarkerPosAndPlaceholderText(); 
	}

	void TextField::edit(SDL_Event& eve) {
		if (!inFocus)
			return;

		// Special key input
		if (eve.type == SDL_KEYDOWN) 
		{
			if (eve.key.keysym.sym == SDLK_BACKSPACE && inputText->getText().length() > 0) // Backspace
				deleteBackspace(); 
			else if (eve.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) // Copy 
				SDL_SetClipboardText(inputText->getText().c_str());
			else if (eve.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) // Paste (klistrar man in f�r mycket tas det l�ngst ut (fram�t) bort
				pasteText(); 
			else if (eve.key.keysym.scancode == SDL_SCANCODE_LEFT)  // Arrow left
				moveMarkerLeft(); 
			else if (eve.key.keysym.scancode == SDL_SCANCODE_RIGHT)  // Arrow right 
				moveMarkerRight(); 
		}
		updateMarkerPosAndPlaceholderText();
	}

	void TextField::addText(SDL_Event& eve) {
		std::string newText = inputText->getText();
		if (isSwedishLetter(eve.key.keysym.sym)) 
			newText += getEnglishLetter(eve.key.keysym.sym); 
		else
			newText += eve.text.text;
		inputText->setText(newText);
	}

	void TextField::insertText(SDL_Event& eve) {
		std::string newText = inputText->getText();
		if (isSwedishLetter(eve.key.keysym.sym)) {
			std::string letter(1, getEnglishLetter(eve.key.keysym.sym)); 
			newText.insert(currentTextIndex, letter); 
		}
		else
			newText.insert(currentTextIndex, eve.text.text);
		inputText->setText(newText);
	}

	void TextField::deleteBackspace() {
		if (currentTextIndex == inputText->getText().length()) { // Ta bort sista karakt�ren 
			std::string newText = inputText->getText();
			newText.pop_back();
			inputText->setText(newText);
		}
		else if (currentTextIndex != 0) { // Ta bort bokstaven d�r mark�ren �r
			std::string newText = inputText->getText();
			newText.erase(currentTextIndex - 1, 1);
			inputText->setText(newText);
		}
		if (currentTextIndex > 0)
			--currentTextIndex;
	}

	void TextField::pasteText() {
		char* pastedText = SDL_GetClipboardText();
		std::string newText = inputText->getText();
		int oldH = inputText->getH(); 
		for (int i = 0; i < strlen(pastedText) && inputText->getW() < getW() - inputText->getHorizontalPadding() - 8; i++) { // Adderar karakt�rer p� r�tt plats tills det blir fullt 
			std::string letter(1, pastedText[i]);
			newText.insert(currentTextIndex, letter);
			inputText->setText(newText);
			++currentTextIndex;
		}

		while (inputText->getH() > oldH) // Tar bort bokst�ver om det blev radbrytning
			deleteBackspace(); 

		SDL_free(pastedText);
	}

	void TextField::moveMarkerLeft() {
		if (currentTextIndex > 0)
			--currentTextIndex;
		textMarker->setActive(true);
		lastUpdate = SDL_GetTicks(); // S� mark�ren inte togglar p� och av medan man flyttar mark�ren (alltid p�)
	}

	void TextField::moveMarkerRight() {
		if (currentTextIndex < inputText->getText().length())
			++currentTextIndex;
		textMarker->setActive(true);
		lastUpdate = SDL_GetTicks();
	}

	void TextField::updateMarkerPosAndPlaceholderText()
	{
		placeholderText->setActive(inputText->getText().empty()); // Aktiv om tom text och vice versa 
		// Mark�r i b�rjan om ej skrivit 
		if (placeholderText->isActive() || currentTextIndex == 0)
			textMarker->setX(getX() + 5);
		else if (currentTextIndex == inputText->getText().length() || (currentTextIndex == 0 && inputText->getText().empty())) // Om mark�ren inte flyttats med piltangenterna dvs skriver i slutet 
			textMarker->setX(inputText->getX() + inputText->getW()); 
		else { // Mark�ren har flyttats med piltangenterna 
			std::string wholeText = inputText->getText(); 
			std::string textToMarker = inputText->getText().substr(0, currentTextIndex); 
			inputText->setText(textToMarker); // S�tter bara texten fram till mark�ren s� jag nedan kan ta reda p� l�ngden f�r den texten och placera mark�ren r�tt 
			textMarker->setX(inputText->getX() + inputText->getW());
			inputText->setText(wholeText); // Skriver sen ut hela texten igen 
		}
	}

	TextField::~TextField()
	{
		// Ty Text objekten �r childs tas de bort automatiskt n�r TextField f�rst�rs, samma med mark�ren och texturen med f�ltets bild som �r fr�n superklassen och tas bort d�r
	}

}