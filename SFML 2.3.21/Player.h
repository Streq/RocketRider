#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "GameObject.h"
#include "Resources.h"
#include "Hook.h"
#include "PlayerDefinition.h"
class Player: public GameObject{
	public:
	typedef	std::unique_ptr<Player> Ptr;
	
	virtual ObjectType		getType()const final;

	Player(const Resources& res, const b2Vec2& position = b2Vec2(0.f, 0.f), float mAcceleration=30.f, float mAngularAcc=1.f, float mRopeLength=15.f, float mMaxFuel=20.f, float mFuel=20.f, float mExplosionImpulse=20.f, float mMaxSpeed = 200.f);
	Player(const Resources& res, PlayerDefinition* def);
	
	void accelerate(sf::Time dt);
	void decelerate();
	void rotateLeft(sf::Time dt);
	void rotateRight(sf::Time dt);
	void throwHook(float x, float y);
	void releaseHook();
	void explode();
	void setGoalCompleted(bool=true);
	bool goalCompleted();
	bool isDead()const;
	bool isHooked()const;
	virtual void initBody(b2World& world) final;


	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse, bool id)final;
	virtual void BeginContact(b2Contact* contact, bool id)final;
	virtual void Step() final;


	private:
	sf::Sprite		mSprite;
	sf::Sprite		mExplosionSprite;
	sf::Sprite		mFire;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const final;
	
	Hook		mHook;
	float		mFuel;
	float		mMaxFuel;
	float		mRopeLength;
	float		mAcceleration;
	float		mMaxSpeed;
	float		mAngularAcc;
	float		mExplosionImpulse;
	
	bool		m_hooked;
	bool		m_accelerating;
	bool		m_explode;
	bool		m_goal;
	bool		m_dead;
	//b2RopeJoint* mHook;
};