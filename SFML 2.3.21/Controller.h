#pragma once
#include <array>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AppContext.h"
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
	Mira,
	zoomin,
	zoomout,
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
	enum Type {not_valid_mane, keyboard, mouse, joy_button, joy_axis };
	
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
		Controller(AppContext context,sf::View& view);
		typedef sf::Keyboard::Key Key;
		typedef std::array<bool, Input::size> KeyBoolSet;
		//typedef unsigned Key;
	private:
		std::array<bool, Input::size>	pressed_keys;
		std::array<bool, Input::size>	just_updated_keys;
		//wether the input is considered just on the instant the key is pressed or the whole duration it's down
		std::array<bool, Input::size>	just_on_update;
		InputData				keys[Input::size];
		sf::View*				mView;

		void					zoom_in();
		void					zoom_out();
	public:
		void					handleEvent(const sf::Event& e);
		void					update_key(const InputData& data, bool pressed);
		void					set_key(const InputData& key, Input::ID action, bool just_on_update = true);
		bool					check_pressed(Input::ID action)const;//check if currently pressed
		bool					check_updated(Input::ID action)const;//check if just updated
		void					updateInput();
		
		const KeyBoolSet&		get_pressed() const;
		const KeyBoolSet&		get_updated() const;
		sf::Vector2f			getPositionOnWorld(sf::Vector2i pos)const;
		sf::Vector2i			lastMouseClick;
		sf::Vector2i			lastMousePosition;
		AppContext				mContext;
};