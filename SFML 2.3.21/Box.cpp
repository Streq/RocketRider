#include "Box.h"
#include "Conversions.h"
ObjectType Box::getType() const
{
	return ObjectType::Box;
}
Box::Box(const Resources& res, const b2Vec2& size, const b2Vec2& position) :
	mSize(size),
	mSprite(res.textures.get(Texture::SPRITE_BOX),
			sf::IntRect(0, 0, size.x*PIXELS_PER_METER, size.y*PIXELS_PER_METER)
	)
{
	auto bounds = mSprite.getLocalBounds();
	sf::Vector2f center(bounds.width / 2.f, bounds.height / 2.f);
	mSprite.setOrigin(center);

	//body definition
	mBodyDef.position = position;
	mBodyDef.type = b2BodyType::b2_dynamicBody;
	mBodyDef.userData = this;

	//fixture definition
	mFixtureDef.friction = 1.f;
	mFixtureDef.density = 0.01f;
	mFixtureDef.restitution = 0.f;
	//shape definition
	b2PolygonShape* bodyShape(new b2PolygonShape);
	bodyShape->SetAsBox(mSize.x / 2.f, mSize.y / 2.f);
	mShape.reset(bodyShape);

	mFixtureDef.shape = bodyShape;


}

Box::Box(const Resources & res, BoxDefinition * def):Box(res,def->size, def->pos)
{
}

void Box::initBody(b2World & world) {
	GameObject::initBody(world);
	mBody = world.CreateBody(&mBodyDef);

	
	mBody->CreateFixture(&mFixtureDef);
}

void Box::draw(sf::RenderTarget & target, sf::RenderStates states) const {

	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite, states);
}
