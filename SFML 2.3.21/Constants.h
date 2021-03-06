#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <Box2D/Box2D.h>
//#define NO_FPS_LIMIT




const sf::Vector2u	ASPECT_RATIO_VEC = sf::Vector2u(10,7);
const float			ASPECT_RATIO = ASPECT_RATIO_VEC.y / (float)ASPECT_RATIO_VEC.x;

const sf::Vector2u	INIT_WINDOW_SIZE = ASPECT_RATIO_VEC * 24u * 4u;
const unsigned		INIT_VIEW_SIZE = ASPECT_RATIO_VEC.x * 24u * 4u;
const unsigned		MIN_VIEW_SIZE = ASPECT_RATIO_VEC.x * 24u * 1u;
const unsigned		MAX_VIEW_SIZE = ASPECT_RATIO_VEC.x * 24u * 24u;

const size_t		FPS = 60;
const unsigned		PIXELS_PER_METER = 32;
const float			METERS_PER_PIXEL = 1.f/PIXELS_PER_METER;		

const int			MOUSE_SCROLL_ZOOM = 1;

namespace B2{

const float32		TIME_STEP = 1.f/60.f;
const b2BodyDef		PLAYER_BODY_DEF;
//The suggested iteration count for Box2D is 8 for velocity and 3 for position
const unsigned		VELOCITY_ITERATIONS=8;
const unsigned		POSITION_ITERATIONS=3;
}

namespace SF{

const sf::Time		TIME_STEP = sf::seconds(B2::TIME_STEP); 
const sf::Time		RENDER_FRAME_TIME = sf::seconds(1.f/FPS);
const sf::Time		FPS_UPDATE_TIME = sf::seconds(1.f);
const sf::Time		LEVEL_TITLE_TIME = sf::seconds(4.f);


}

namespace Color{
	const sf::Color Brown = sf::Color(104, 47, 6);
	const sf::Color Dark_Green = sf::Color(12, 73, 2);
	const sf::Color Super_Dark_Green = sf::Color(0,30,0);
	const sf::Color Dark_Blue = sf::Color(1, 5, 81);
	const sf::Color Grey = sf::Color(128,128,128);
	const sf::Color Orange = sf::Color(255,165,0);
	const sf::Color Golden = sf::Color(218,165,32);
}

enum class ObjectType { Player, Box, Block, Goal, DeathBlock, size };
