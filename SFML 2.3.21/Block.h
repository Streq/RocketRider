#pragma once
#include "GameObject.h"
#include "Resources.h"
#include "Constants.h"
struct BlockDefinition;
class Block: public GameObject{
	public:
	Block(const Resources& res,const b2Vec2& size=b2Vec2(1.f,1.f), const b2Vec2& position=b2Vec2(0.f,0.f));
	Block(const Resources& res,BlockDefinition* def);
	virtual void		initBody(b2World& world);
	virtual ObjectType	getType()const;
	
	
	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const;
	sf::Sprite			mSprite;
	b2Vec2				mSize;


};