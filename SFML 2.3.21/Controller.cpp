#include "Controller.h"
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
