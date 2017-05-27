#pragma once
#include "Menu.h"
class PauseMenu :public Menu {
public:
	PauseMenu(GameStack& s, AppContext c);
	virtual bool handle_event(const sf::Event& e) override;
};