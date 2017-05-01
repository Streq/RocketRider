#include "Menu.h"


Menu::Menu(GameStack & s, AppContext c):
GameState(s,std::move(c))
{;
	
	
}

//draw each element
void Menu::draw() const
{
	mContext.screen->draw(mElements);
}

bool Menu::handle_event(const sf::Event & e)
{
	mElements.handleEvent(e, *mContext.window);
	return false;
}

bool Menu::update(sf::Time tiempo)
{
	return false;
}

GUI::Composite & Menu::getElements()
{
	return mElements;
}

