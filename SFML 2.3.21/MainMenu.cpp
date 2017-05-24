#include "MainMenu.h"

MainMenu::MainMenu(GameStack & s, AppContext c):
	Menu(s,c)
{
	addText("Rocket Riders", 40, 40u);
	addText("un juego medio choti", 40u, 20u);
	addButton("Jugar",
		[this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::GAME_MODE);
		}
	); 
	addButton("Como se juega esto",
		[this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::HOW_TO_PLAY);
		}
	); 
	addButton("Bueno santi te dejo",
		[this]() {
			this->requestStackClear();
		}
	);
	addText(mContext.resources->texts.get(TextFile::CREDITS),50u);

	/*
	{
		auto* bot = new GUI::Button(mContext, "Jugar");
		bot->setCallback([this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::GAME_MODE);
		});
		bot->setPosition(50.f, 50.f);
		mElements.addChild(GUI::Node::ptr(bot));
	} 
	{
		auto* bot = new GUI::Button(mContext, "Como pito se maneja esto");
		bot->setCallback([this]() {
			this->requestStackPop();
			this->requestStackPush(GameState::ID::HOW_TO_PLAY);
		});
		bot->setPosition(50.f, 120.f);
		mElements.addChild(GUI::Node::ptr(bot));
	}
	{
		auto* bot = new GUI::Button(mContext, "Salir");
		bot->setCallback([this]() {
			this->requestStackClear();
		});
		bot->setPosition(50.f, 190.f);
		mElements.addChild(GUI::Node::ptr(bot));
	}
	{
		auto* bot = new GUI::TextBox(mContext, mContext.resources->texts.get(TextFile::CREDITS), 50u);
		bot->setPosition(50.f, 260.f);
		mElements.addChild(GUI::Node::ptr(bot));
	}
	*/
}
