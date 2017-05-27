#include "Controller.h"
Controller::Controller(AppContext context):mContext(std::move(context))
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
			if (e.mouseButton.button == sf::Mouse::Button::Left) {
				//get local click
				input[Input::Hook] = true;

				lastMouseClick = sf::Vector2i(e.mouseButton.x, e.mouseButton.y);

			}
			if (e.mouseButton.button == sf::Mouse::Button::Right) {
				input[Input::ReleaseHook] = true;

			}
		}break;
		case sf::Event::MouseButtonReleased: {
			if (e.mouseButton.button == sf::Mouse::Button::Left) {
				input[Input::Hook] = false;
			}
			if (e.mouseButton.button == sf::Mouse::Button::Right) {
				input[Input::ReleaseHook] = false;

			}
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

void Controller::set_key(const InputData& key, Input::ID action) {
	keys[action] = key;
}

bool Controller::check_pressed(Input::ID i) const { return pressed_keys[i]; }
bool Controller::check_updated(Input::ID i) const { return just_updated_keys[i]; }
void Controller::clear_updated() {
	just_updated_keys.fill(false);
}
const Controller::KeyBoolSet& Controller::get_pressed() const{ return pressed_keys; };
const Controller::KeyBoolSet& Controller::get_updated() const{ return just_updated_keys; };
