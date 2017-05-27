#include "PauseMenu.h"

PauseMenu::PauseMenu(GameStack & s, AppContext c)
	:Menu(s,std::move(c))
{
	this->mElements.setPosition(0.f,0.f);
	addButton("Volver al Menú", [this]() {
		this->requestStackPop();
		this->requestStackPop();
		this->requestStackPush(GameState::ID::MAIN_MENU);
	});
	addButton("Volver al juego", [this]() {
		this->requestStackPop();
	});
	addButton("Ver videos de yayo en youtube", [this]() {
		this->requestStackClear();
	});
}

bool PauseMenu::handle_event(const sf::Event & e)
{

	switch (e.type) {
		case sf::Event::KeyPressed: {
			if (e.key.code == sf::Keyboard::Return) {
				requestStackPop();
				return false;
			}
		}
	}
	Menu::handle_event(e);
	return false;
}

