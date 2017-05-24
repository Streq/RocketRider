#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include <sstream>
#include <math.h>
#include "SFML/Main.hpp"
#include "rrmath.h"
#include "Circle.h"

/**
Takes a b2Vector in Box2D World Coordinates and translates it to a sf::Vector2f in SFML World Coordinates
Note: for size conversions use b2_to_sf_size
*/
inline sf::Vector2f b2_to_sf_pos(const b2Vec2& b2v){
	return sf::Vector2f(b2v.x*PIXELS_PER_METER, -b2v.y*PIXELS_PER_METER);
}
/**
Takes a b2Vector in Box2D Size Coordinates and translates it to a sf::Vector2f in SFML Size Coordinates
*/
inline sf::Vector2f b2_to_sf_size(const b2Vec2& b2v){
	return sf::Vector2f(b2v.x*PIXELS_PER_METER, b2v.y*PIXELS_PER_METER);
}

/**
Takes a sf::Vector2f in SFML World Coordinates and translates it to a b2Vector in Box2D World Coordinates
Note: for size conversions use sf_to_b2_size
*/
inline b2Vec2 sf_to_b2_pos(const sf::Vector2f& sfv){
	return b2Vec2(sfv.x*METERS_PER_PIXEL, -sfv.y*METERS_PER_PIXEL);
}
/**
Takes a sf::Vector2f in SFML Size Coordinates and translates it to a b2Vector in Box2D Size Coordinates
*/
inline b2Vec2 sf_to_b2_size(const sf::Vector2f& sfv) {
	return b2Vec2(sfv.x*METERS_PER_PIXEL, sfv.y*METERS_PER_PIXEL);
}

#define deg_to_rad(deg) (deg/180.f*b2_pi)

#define rad_to_deg(rad) (rad*180.f/b2_pi)

/**
Takes an angle in radians and a magnitude and converts them to a b2Vector
*/
inline b2Vec2 rad_to_b2Vec(float rad, float mag = 1.f){
	return b2Vec2(cos(rad)*mag,sin(rad)*mag);
}

/**
Takes an angle in degrees and a magnitude and converts them to a b2Vector
*/
inline b2Vec2 deg_to_b2Vec(float deg, float mag = 1.f){
	return rad_to_b2Vec(deg_to_rad(deg), mag);
}

/**
Takes an angle in radians and a magnitude and converts them to a sf::Vector2f
*/
inline sf::Vector2f rad_to_sfVec(float rad, float mag = 1.f){
	return sf::Vector2f(cos(rad)*mag,-sin(rad)*mag);
}

/**
Takes an angle in degrees and a magnitude and converts them to a sf::Vector2f
*/
inline sf::Vector2f deg_to_sfVec(float deg, float mag = 1.f){
	return rad_to_sfVec(deg_to_rad(deg), mag);
}




/**
Maps a pixel to the specified coord system
@param win the window the pixel is relative to
@param trans the transform that will be inversely applied to get the local coords
@param coords the coords of the pixel relative to win
*/
inline sf::Vector2f pixelToLocalCoords(sf::RenderTarget& win, sf::Transform trans, const sf::Vector2i& coords) {
	auto worldCoords = win.mapPixelToCoords(coords);
	return trans.getInverse() * worldCoords;
}



/**
Maps target pixel to specified sprite coords.

@param win the render target the pixel are relative to
@param sprite the sprite with the coord system of the return vector
@param coords the pixel coords relative to win
@returns the coords (in pixels) relative to the sprite
*/
inline sf::Vector2i windowToSpriteCoords(sf::RenderTarget& win, sf::Sprite& sprite, const sf::Vector2i& coords) {

	auto localCoords = pixelToLocalCoords(win, sprite.getTransform(), coords);

	return win.mapCoordsToPixel(localCoords);
}




/**
Makes a string in hexadecimal format from an uint32
*/
inline std::string u32_to_hex_string(uint32 i)
{
	std::stringstream stream;
	stream << std::hex << i;
	return stream.str();
}



inline ObjectType type_from_string(const std::string& obj) {
	if (obj.compare("Player") == 0) {
		return ObjectType::Player;
	}if (obj.compare("Box") == 0) {
		return ObjectType::Box;
	}if (obj.compare("Block") == 0) {
		return ObjectType::Block;
	}if (obj.compare("Goal") == 0) {
		return ObjectType::Goal;
	}if (obj.compare("DeathBlock") == 0) {
		return ObjectType::DeathBlock;
	}
	return ObjectType::size;

};

inline std::string type_to_string(ObjectType type) {
	switch (type) {
		case ObjectType::Player:
			return "Player";
		case ObjectType::Box:
			return "Box";
		case ObjectType::Block:
			return "Block";
		case ObjectType::Goal:
			return "Goal";
		case ObjectType::DeathBlock:
			return "DeathBlock";
	}

	return "";
}

/**
@brief converts a sf::View to sf::FloatRect
@param view the view
@returns sf::FloatRect in the same coord system of the view
*/
inline sf::FloatRect viewRect(const sf::View& view) { 
	return sf::FloatRect(view.getCenter() - view.getSize()*0.5f, view.getSize());
}

/**
@brief converts a sf::View to sf::FloatRect taking its rotation into account
@param view the view
@returns the axis aligned bounding rect of the view
*/
inline sf::FloatRect rotatedViewRect(const sf::View& view) {
	auto size = view.getSize();
	sf::FloatRect rect(view.getCenter() - size * 0.5f, size);
	/*
	sf::Transform trans;
	trans.translate(size*0.5f);//set origin
	trans.rotate(view.getRotation());//rotate (otherwise we would be rotating from the top left corner)
	trans.translate(-size*0.5f);//reset origin
	*/
	sf::Transformable t;
	t.setOrigin(rectCenter(rect));//set the origin, which also alters the position
	t.setPosition(view.getCenter());//set the position to even out the setOrigin's position change
	t.rotate(view.getRotation());//rotate that shit TIGHT
	return t.getTransform().transformRect(rect);//return the transformed rect
	
}



/**
Returns the bounding circle of a sf::FloatRect
*/
inline Circle getBoundingCircle(const sf::FloatRect& rect) {
	return Circle(vec_length(sf::Vector2f(rect.width, rect.height)) * 0.5f, rectCenter(rect));
}