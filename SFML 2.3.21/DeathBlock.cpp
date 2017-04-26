#include "DeathBlock.h"

#include "BlockDefinition.h"
#include "Conversions.h"
#include "Player.h"
void DeathBlock::initBody(b2World & world)
{

	//body definition

	GameObject::initBody(world);

	mBody = world.CreateBody(&mBodyDef);


	mBody->CreateFixture(&mFixtureDef);
	
}
ObjectType DeathBlock::getType() const
{
	return ObjectType::DeathBlock;
}
void DeathBlock::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite, states);
}

DeathBlock::DeathBlock(const Resources& res, const b2Vec2& size, const b2Vec2& position) :
	mSize(size),
	mSprite(res.textures.get(Texture::SPRITE_LAVA),
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

	//shape definition
	b2PolygonShape* bodyShape(new b2PolygonShape);
	bodyShape->SetAsBox(mSize.x / 2.f, mSize.y / 2.f);
	mShape.reset(bodyShape);

	mFixtureDef.shape = bodyShape;

}

DeathBlock::DeathBlock(const Resources & res, BlockDefinition * def) : DeathBlock(res, def->size, def->pos)
{
}

void DeathBlock::BeginContact(b2Contact * contact, bool id)
{
	b2Fixture* fix = id ? contact->GetFixtureA() : contact->GetFixtureB();
	GameObject* obj = static_cast<GameObject*>(fix->GetBody()->GetUserData());
	ObjectType type = obj->getType();
	switch (type) {
		case ObjectType::Player: {
			static_cast<Player*>(obj)->explode();
		}break;
	}
}