#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
#include "Constants.h"

inline sf::Vector2f b2_to_sf_pos(const b2Vec2& b2v){
	return sf::Vector2f(b2v.x*PIXELS_PER_METER, -b2v.y*PIXELS_PER_METER);
}

inline sf::Vector2f b2_to_sf_size(const b2Vec2& b2v){
	return sf::Vector2f(b2v.x*PIXELS_PER_METER, b2v.y*PIXELS_PER_METER);
}
inline b2Vec2 sf_to_b2_pos(const sf::Vector2f& sfv){
	return b2Vec2(sfv.x*METERS_PER_PIXEL, -sfv.y*METERS_PER_PIXEL);
}

inline b2Vec2 sf_to_b2_size(const sf::Vector2f& sfv){
	return b2Vec2(sfv.x*METERS_PER_PIXEL, sfv.y*METERS_PER_PIXEL);
}

#define deg_to_rad(deg) (deg/180.f*b2_pi)

#define rad_to_deg(rad) (rad*180.f/b2_pi)


inline b2Vec2 rad_to_b2Vec(float rad, float mag = 1.f){
	return b2Vec2(cos(rad)*mag,sin(rad)*mag);
}

inline b2Vec2 deg_to_b2Vec(float deg, float mag = 1.f){
	return rad_to_b2Vec(deg_to_rad(deg), mag);
}

inline sf::Vector2f rad_to_sfVec(float rad, float mag = 1.f){
	return sf::Vector2f(cos(rad)*mag,-sin(rad)*mag);
}

inline sf::Vector2f deg_to_sfVec(float deg, float mag = 1.f){
	return rad_to_sfVec(deg_to_rad(deg), mag);
}
