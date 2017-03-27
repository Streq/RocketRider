#pragma once
#include <array>
#include <SFML/System.hpp>
namespace Input{
enum ID{
	Left,
	Right,
	Accelerate,
	Hook,
	ReleaseHook,
	size
};
}

struct Controller{
	std::array<bool,Input::size> input;
	sf::Vector2i lastMouseClick;
};