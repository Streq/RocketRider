#include "MenuModo.h"

MenuModo::MenuModo(GameStack & s, AppContext c) :
	Menu(s, c)
{
	addText("ELIJA UN MODO DE JUGO", 40u,40u);

	addText("", 40u);


	addButton("UN JUGADOR",
		[this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::GAME_SINGLE);
			auto& set = *this->mContext.settings;
			set.players = 1;
			set.modo = GameSettings::Modo::single;
		}
	);

	addButton("MULTIJUGADOR",
		[this]() {
		this->requestStackPop();
		this->requestStackPush(GameState::ID::MULTI_PLAYER_MENU);
		auto& set = *this->mContext.settings;
		set.players = 2;
		set.modo = GameSettings::Modo::race;
		set.rounds = 3;
		set.mapa = 0;
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
