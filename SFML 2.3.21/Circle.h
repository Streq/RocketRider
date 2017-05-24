#pragma once
#include <SFML\Graphics.hpp>
#include "rrmath.h"
struct Circle {
public:
	float radius;
	sf::Vector2f position;
	Circle(float radius, sf::Vector2f position) :radius(radius), position(position) {}
public:
	bool isInside(sf::Vector2f pos) {
		return vec_length_squared(pos - position)<radius*radius;
	}
};