#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "GameObject.h"
#include "Resources.h"
class Player: public GameObject{
	public:
	typedef	std::unique_ptr<Player> Ptr;

	Player(const Resources& res, float mAcceleration=20.f, float mAngularAcc=1.f, float mRopeLength=10.f, float mMaxFuel=20.f, float mFuel=20.f);
	void accelerate(sf::Time dt);
	void decelerate();
	void rotateLeft(sf::Time dt);
	void rotateRight(sf::Time dt);
	void throwHook(float x, float y);
	void releaseHook();
	bool isHooked()const;
	virtual void initBody(b2World& world, const b2Vec2& pos);

	
	private:
	sf::Sprite		mSprite;
	sf::Sprite		mFire;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	
	
	float		mFuel;
	float		mMaxFuel;
	float		mRopeLength;
	float		mAcceleration;
	float		mSpeed;
	float		mAngularAcc;
	bool		mIsHooked;
	bool		mIsAccelerating;
	b2RopeJoint* mHook;
};