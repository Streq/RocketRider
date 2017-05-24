#pragma once
#include <array>
#include <SFML/System.hpp>
#include <SFML/Window/Keyboard.hpp>
namespace Input{
enum ID{
	Left,
	Right,
	Accelerate,
	Hook,
	Hookup,
	Hookdown,
	ReleaseHook,
	Die,
	size
};
}

namespace JoyButtons {
	enum Button {
		A,
		B,
		X,
		Y,
		LB,
		RB,
		Back,
		Start,
		size
	};
}


struct InputData {
	enum Type { keyboard, joy_button, joy_axis };
	
	//key code
	unsigned code;
	//input type
	Type type;
	//Joystick
	unsigned joy_ID;
	//position in the joy_axis
	int pos_axis;

	InputData() {};
	InputData(unsigned code, Type type, unsigned joy_ID = 0, int pos_axis = 0)
		:code(code)
		, type(type)
		, joy_ID(joy_ID)
		, pos_axis(pos_axis) {
	}
	inline bool operator==(const InputData& r) const {
		return
			code == r.code &&
			type == r.type &&
			joy_ID == r.joy_ID &&
			pos_axis == r.pos_axis;
	}


};


struct Controller{
		std::array<bool,Input::size> input;
	public:
		typedef sf::Keyboard::Key Key;
		typedef std::array<bool, Input::size> KeyBoolSet;
		//typedef unsigned Key;
	private:
		std::array<bool,Input::size>	pressed_keys;
		std::array<bool, Input::size>	just_updated_keys;
		InputData				keys[Input::size];
	public:
		void					update_key(const InputData& data, bool pressed);
		void					set_key(const InputData& key, Input::ID action);
		bool					check_pressed(Input::ID action)const;//check if currently pressed
		bool					check_updated(Input::ID action)const;//check if just updated
		void					clear_updated();
		const KeyBoolSet&		get_pressed() const;
		const KeyBoolSet&		get_updated() const;
		sf::Vector2i lastMouseClick;
};