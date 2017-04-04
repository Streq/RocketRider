#include "Goal.h"

#include "Conversions.h"
Goal::Goal(const Resources & res, const b2Vec2 & size, const b2Vec2& position):
	mSize(size),
	mSprite(
		res.textures.get(Texture::SPRITE_GOAL),
		sf::IntRect(0, 0, size.x*PIXELS_PER_METER, size.y*PIXELS_PER_METER)
	)
{
	auto bounds = mSprite.getLocalBounds();
	sf::Vector2f center(bounds.width / 2.f, bounds.height / 2.f);
	mSprite.setOrigin(center);

	mBodyDef.position = position;
	mBodyDef.type = b2BodyType::b2_staticBody;
	mBodyDef.userData = this;

	//fixture definition
	mFixtureDef.friction = 1.f;
	mFixtureDef.isSensor = true;

	//shape definition
	b2PolygonShape* bodyShape(new b2PolygonShape);
	bodyShape->SetAsBox(mSize.x / 2.f, mSize.y / 2.f);

	mShape.reset(bodyShape);

	mFixtureDef.shape = bodyShape;


}


void Goal::initBody(b2World & world) {
	//body definition

	GameObject::initBody(world);
	mBody = world.CreateBody(&mBodyDef);

	mBody->CreateFixture(&mFixtureDef);
}

GameObject::Type Goal::getType() const
{
	return Type::Goal;
}

void Goal::draw(sf::RenderTarget & target, sf::RenderStates states) const {

	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite, states);
}

