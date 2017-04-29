#pragma once
#include "ResourceHolder.h"
#include <SFML/Graphics.hpp>
namespace Font {
enum ID {
	arial,
	consola
};
}


typedef ResourceHolder<sf::Font, Font::ID> FontHolder;
