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

	Player(const Resources& res, const b2Vec2& position = b2Vec2(0.f, 0.f), float mAcceleration=30.f, float mAngularAcc=1.f, float mRopeLength=15.f, float mMaxFuel=20.f, float mFuel=20.f, float mExplosionImpulse=20.f, float mMaxSpeed = 200.f, bool m_always_accelerating = false, bool has_fuel = true, bool has_rope = true, bool has_steer = true);
	Player(const Resources& res, PlayerDefinition* def);
	
	void accelerate(sf::Time dt);
	void decelerate();
	void rotateLeft(sf::Time dt);
	void rotateRight(sf::Time dt);
	void throwHookTowardsWorldPosition(float x, float y);
	void throwHookTowardsLocalDirection(float x, float y);
	void releaseHook();
	void explode();
	void setGoalCompleted(bool=true);
	bool goalCompleted();
	bool isDead()const;
	bool isHooked()const;
	void updateAimTowardsWorldPosition(float x,float y);
	void setMira(bool mira);
	bool getMira();
	virtual void initBody(b2World& world) final;


	bool hasRope()const;
	bool hasSteer()const;
	bool hasFuel()const;

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse, bool id)final;
	virtual void BeginContact(b2Contact* contact, bool id)final;
	virtual void Step(sf::Time dt) final override;


	private:
	sf::Sprite		mSprite;
	sf::Sprite		mExplosionSprite;
	sf::Sprite		mFire;
	mutable sf::VertexArray aimLine;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const final;
	void		 drawAim(sf::RenderTarget& target, sf::RenderStates states) const;
	b2Vec2	getDirectionTowards(float x, float y);


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
	bool		m_always_accelerating;
	bool		m_explode;
	bool		m_goal;
	bool		m_dead;
	bool		m_mira;

	bool has_fuel;
	bool has_rope;
	bool has_steer;

	b2Vec2		mAim;
	//b2RopeJoint* mHook;
};