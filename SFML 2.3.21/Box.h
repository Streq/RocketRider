#pragma once
#include "GameObject.h"
#include "Resources.h"
#include "Constants.h"
#include "BoxDefinition.h"
class Box : public GameObject {
	public:
	virtual ObjectType		getType()const;
	Box(const Resources& res, const b2Vec2& size = b2Vec2(1.f, 1.f), const b2Vec2& position = b2Vec2(0.f, 0.f));
	Box(const Resources& res, BoxDefinition* def);

	virtual void		initBody(b2World& world);
	
	
	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const;
	sf::Sprite			mSprite;
	b2Vec2				mSize;


};