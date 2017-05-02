#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
#include "Constants.h"
#include <sstream>
#include <math.h>
/**
Takes a Vector and returns a floored copy
*/
inline sf::Vector2f floor(const sf::Vector2f& vec) {
	return sf::Vector2f(floor(vec.x), floor(vec.y));
}
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
Takes a number and returns its sign
@returns int
-1 if val < 0
1 if val > 0
0 if val == 0
*/
inline int sign(float val){
	return (0.f < val) - (val < 0.f);
}
/**
Takes a number and returns its sign
@returns int
-1 if val < 0
1 if val > 0
0 if val == 0
*/
inline int sign(int val){
	return (0 < val) - (val < 0);
}
/**
Takes a number and returns its sign
@retval -1 if val < 0
@retval 1 if val > 0 
@retval 0 if val == 0
*/
inline int sign(double val){
	return (0 < val) - (val < 0);
}

/**
Returns the squared length of a sf::Vector2f
*/
inline float vec_length_squared(const sf::Vector2f& vec) {
	return vec.x*vec.x + vec.y*vec.y;
}
/**
Returns the length of a sf::Vector2f
*/
inline float vec_length(const sf::Vector2f& vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

/**
Takes a sf::Vector2f and sets its length to 1.
@warning if the vector's length is 0 it is left unmodified
@param vec the vector to modify
@returns the former length of the vector 
*/
inline float normalize(sf::Vector2f& vec){
	float length = vec_length(vec);
	
	if (length < FLT_EPSILON) return 0.f;
	
	float inverse_length = 1.f / length;
	vec *= inverse_length;
	return length;
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
inline sf::FloatRect viewRect(const sf::View& view) { return sf::FloatRect(view.getCenter() - view.getSize()*0.5f, view.getSize()); }

/**
@brief get the intersection Rect of two given FloatRects
*/
inline sf::FloatRect getIntersection(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
	float left = std::max(rect1.left, rect2.left);
	float top = std::max(rect1.top, rect2.top);
	float width = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - left;
	float height = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - top;
	return sf::FloatRect(left, top, width, height);
};

/**
Returns the center of a sf::FloatRect
*/
inline sf::Vector2f rectCenter(const sf::FloatRect& rect){
	return sf::Vector2f(rect.left + rect.width*0.5f, rect.top + rect.height*0.5f);
}