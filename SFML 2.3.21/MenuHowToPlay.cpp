#include "MenuHowToPlay.h"

MenuHowToPlay::MenuHowToPlay(GameStack & s, AppContext c) :
	Menu(s, c)
{
	addButton("Jugar", [this]() {
		this->requestStackPop();
		this->requestStackPush(GameState::ID::GAME_MODE);
	});
	addButton("Volver", [this]() {
		this->requestStackPop();
		this->requestStackPush(GameState::ID::MAIN_MENU);
	});
	addText(mContext.resources->texts.get(TextFile::HOWTOPLAY),50u);
}

