#include "Controller.h"
Controller::Controller(AppContext context, const sf::View& view):mContext(std::move(context)),mView(&view)
{
}
void Controller::handleEvent(const sf::Event & e)
{
	switch(e.type){
		case sf::Event::KeyReleased:
		case sf::Event::KeyPressed: {
			bool pressed = e.type == sf::Event::KeyPressed;
			update_key(InputData(e.key.code, InputData::Type::keyboard), pressed);

		}break;
		case sf::Event::MouseButtonPressed: {
			update_key(InputData(e.mouseButton.button, InputData::Type::mouse), true);
			lastMouseClick = sf::Vector2i(e.mouseButton.x, e.mouseButton.y);

		}break;
		case sf::Event::MouseButtonReleased: {
			update_key(InputData(e.mouseButton.button, InputData::Type::mouse), false);
		}break;
		case sf::Event::MouseMoved: {
			lastMousePosition = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
			
		}break;
	}
}
void Controller::update_key(const InputData& data, bool pressed) {
	for (size_t i = 0; i < Input::size; i++) {
		if (data == keys[i]) {
			just_updated_keys[i] = pressed_keys[i] != pressed;
			pressed_keys[i] = pressed;
		}
	}
}

void Controller::set_key(const InputData& key, Input::ID action, bool just_on_update) {
	keys[action] = key;
	this->just_on_update[action] = just_on_update;
}

bool Controller::check_pressed(Input::ID i) const { return pressed_keys[i]; }
bool Controller::check_updated(Input::ID i) const { return just_updated_keys[i]; }
void Controller::updateInput()
{
	for (auto i = 0; i < Input::size; ++i) {
		input[i] = pressed_keys[i] && (just_updated_keys[i] || !just_on_update[i]);
	}
	just_updated_keys.fill(false);
}
const Controller::KeyBoolSet& Controller::get_pressed() const{ return pressed_keys; };
const Controller::KeyBoolSet& Controller::get_updated() const{ return just_updated_keys; }
sf::Vector2f Controller::getPositionOnWorld(sf::Vector2i pos)const
{
	return mContext.window->mapPixelToCoords(pos, *mView);
}

