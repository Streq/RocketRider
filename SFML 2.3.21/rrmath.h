#pragma once
#include <SFML\Graphics.hpp>
#include <math.h>
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
inline sf::Vector2f rectCenter(const sf::FloatRect& rect) {
	return sf::Vector2f(rect.left + rect.width*0.5f, rect.top + rect.height*0.5f);
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
inline float normalize(sf::Vector2f& vec) {
	float length = vec_length(vec);

	if (length < FLT_EPSILON) return 0.f;

	float inverse_length = 1.f / length;
	vec *= inverse_length;
	return length;
}


/**
Takes a number and returns its sign
@returns int
-1 if val < 0
1 if val > 0
0 if val == 0
*/
inline int sign(float val) {
	return (0.f < val) - (val < 0.f);
}
/**
Takes a number and returns its sign
@returns int
-1 if val < 0
1 if val > 0
0 if val == 0
*/
inline int sign(int val) {
	return (0 < val) - (val < 0);
}
/**
Takes a number and returns its sign
@retval -1 if val < 0
@retval 1 if val > 0
@retval 0 if val == 0
*/
inline int sign(double val) {
	return (0 < val) - (val < 0);
}
/**
Takes a Vector and returns a floored copy
*/
inline sf::Vector2f floor(const sf::Vector2f& vec) {
	return sf::Vector2f(floor(vec.x), floor(vec.y));
}
/**
Multiplies each member of vector A by the corresponding member of vector B
example:
(1,4)*(2,3)=(1*2,4*3)=(2,12)
*/
inline sf::Vector2f scaleVector(const sf::Vector2f& a, const sf::Vector2f& b) {
	return sf::Vector2f(a.x*b.x, a.y*b.y);
}


inline float positive_modulo(float a, float b) {
	return fmod(b + fmod(a, b),b);
}
