#pragma once
#include "ResourceHolder.h"
#include <SFML/Graphics.hpp>
namespace Texture {
	enum ID {
		SPRITE_PLAYER_1,
		SPRITE_PLAYER_2,
		SPRITE_PLAYER_3,
		SPRITE_BLOCK,
		SPRITE_TILE,
		SPRITE_FIRE,
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
