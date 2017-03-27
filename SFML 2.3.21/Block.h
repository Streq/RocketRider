#pragma once
#include "GameObject.h"
#include "Resources.h"
#include "Constants.h"
class Block: public GameObject{
	public:
	Block(const Resources& res,const b2Vec2& size=b2Vec2(1.f,1.f));
	virtual void		initBody(b2World& world,const b2Vec2& position);
	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const;
	sf::Sprite			mSprite;
	b2Vec2				mSize;


};