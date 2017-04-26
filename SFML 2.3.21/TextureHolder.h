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
		SPRITE_BOX,
		SPRITE_FIRE,
		SPRITE_EXPLOSION,
		SPRITE_GOAL,
		SPRITE_LAVA,
		SPRITE_BACKGROUND,
		TILESET,
		STARS,
		STARS1,
		STARS2
	};
}


typedef ResourceHolder<sf::Texture, Texture::ID> TextureHolder;
