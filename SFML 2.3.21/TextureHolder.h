#pragma once
#include "ResourceHolder.h"
#include <SFML/Graphics.hpp>
namespace Texture {
	enum ID {
		PLAYER_1,
		PLAYER_2,
		PLAYER_3,
		BLOCK,
		TILE,
		BOX,
		FIRE,
		EXPLOSION,
		GOAL,
		LAVA,
		BUTTON,
		BUTTON_MOUSEOVER,
		BACKGROUND,
		TILESET,
		STARS,
		STARS1,
		STARS2,
		SKY,
		CLOUDS,
		SEA,
		MOUNTAIN0,
		MOUNTAIN1,
		SEA_BLOCK
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
