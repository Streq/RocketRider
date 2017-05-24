#include "MenuModo.h"

MenuModo::MenuModo(GameStack & s, AppContext c) :
	Menu(s, c)
{
	addText("ELIJA UN MODO DE JUGO", 40u,40u);

	addText("", 40u);


	addButton("MODO CRAZY",
		[this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::GAME);
		}
	);
	addButton("me vuelvo al otro menu taba mas tranqui",
		[this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::MAIN_MENU);
		}
	);
	addText("ALTOS MODOS COMING SOON", 40u);

	
}
