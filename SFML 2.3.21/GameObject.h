#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <memory>
#include "Resources.h"
#include "Constants.h"

class GameObject: public sf::Drawable{

	public:
	~GameObject();
	
	typedef	std::unique_ptr<GameObject> Ptr;


	const b2Vec2& getb2Position()const;
	//in radians
	float getb2Rotation()const;

	virtual void		initBody(b2World& world)=0;
	b2Body*				getBody()const;
	b2World*			getWorld()const;
	virtual ObjectType	getType()const=0;

	public:
	//contact handling (cool stuff!!)
	virtual void BeginContact(b2Contact* contact, bool id){}
	virtual void EndContact(b2Contact* contact, bool id){}
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold, bool id){}
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse, bool id){}
	
	//Game step logic b
	virtual void Step() {}


	protected:
	
	b2Body*					 mBody;
	b2World*				 mWorld;
	b2BodyDef				 mBodyDef;
	b2FixtureDef			 mFixtureDef;
	std::unique_ptr<b2Shape> mShape;
};
