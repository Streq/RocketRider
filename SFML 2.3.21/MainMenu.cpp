#include "MainMenu.h"

MainMenu::MainMenu(GameStack & s, AppContext c):
	Menu(s,c)
{
	{
		auto* bot = new GUI::Button(mContext, "Jugar");
		bot->setCallback([this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::GAME);
		});
		bot->setPosition(50.f, 50.f);
		mElements.addChild(GUI::Node::ptr(bot));
	} 
	{
		auto* bot = new GUI::Button(mContext, "Salir");
		bot->setCallback([this]() {
			this->requestStackClear();
		});
		bot->setPosition(50.f, 120.f);
		mElements.addChild(GUI::Node::ptr(bot));
	}
}
