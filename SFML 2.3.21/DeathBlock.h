#pragma once
#include "GameObject.h"
#include "Resources.h"
#include "Constants.h"
struct BlockDefinition;
class DeathBlock : public GameObject {
	public:
	DeathBlock(const Resources& res, const b2Vec2& size = b2Vec2(1.f, 1.f), const b2Vec2& position = b2Vec2(0.f, 0.f));
	DeathBlock(const Resources& res, BlockDefinition* def);
	void		BeginContact(b2Contact * contact, bool id) override;
	void		initBody(b2World& world) final;
	ObjectType	getType()const final;

	//virtual sf::FloatRect	getGlobalBounds() const;

	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const;
	//sf::Sprite			mSprite;
	b2Vec2				mSize;


};