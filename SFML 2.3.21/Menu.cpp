#include "Menu.h"


Menu::Menu(GameStack & s, AppContext c):
GameState(s,std::move(c))
{;
	
	
}

//draw each element
void Menu::draw() const
{
	mContext.screen->setView(mContext.screen->getDefaultView());
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

void Menu::addButton(std::string name, std::function<void()> callback) {
	auto button = std::make_unique<GUI::Button>(this->mContext, name);
	button->setCallback(callback);
	addElement(std::move(button), 50.f, 20.f);
}
void Menu::addText(std::string text, unsigned lineWidth, unsigned char_size) {
	auto Text = std::make_unique<GUI::TextBox>(this->mContext, text, lineWidth, char_size);
	
	addElement(std::move(Text), 50.f, 20.f);
};

GUI::Composite & Menu::getElements()
{
	return mElements;
}

void Menu::addElement(GUI::Node::ptr ptr, float x_offset, float y_offset) {
	auto bounds = mElements.getGlobalBounds();
	ptr->setPosition(50.f, bounds.top + bounds.height + 20.f);
	mElements.addChild(std::move(ptr));
};

