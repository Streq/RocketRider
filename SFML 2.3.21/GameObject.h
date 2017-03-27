#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <memory>
class GameObject: public sf::Drawable{
	

	public:
	typedef	std::unique_ptr<GameObject> Ptr;

	const b2Vec2& getb2Position()const;
	//in radians
	float getb2Rotation()const;

	virtual void		initBody(b2World& world,const b2Vec2& position)=0;
	
	protected:
	
	b2Body*				mBody;
	b2World*			mWorld;
};
