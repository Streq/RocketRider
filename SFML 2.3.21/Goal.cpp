#include "Goal.h"

#include "Conversions.h"
Goal::Goal(const Resources & res, Goal::Callback callback, const b2Vec2 & size, const b2Vec2& position):
	mSize(size),
	mCallback(callback),
	mSprite(
		res.textures.get(Texture::GOAL),
		sf::IntRect(0, 0, static_cast<int>(size.x*PIXELS_PER_METER), static_cast<int>(size.y*PIXELS_PER_METER))
	),
	m_activate(false)
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

Goal::Goal(const Resources & res,const Goal::Callback callback, GoalDefinition *def):
	Goal(res,callback,def->size,def->pos)
{
}


void Goal::initBody(b2World & world) {
	//body definition

	GameObject::initBody(world);
	mBody = world.CreateBody(&mBodyDef);

	mBody->CreateFixture(&mFixtureDef);
}

ObjectType Goal::getType() const
{
	return ObjectType::Goal;
}

void Goal::Step(sf::Time dt)
{
	if (m_activate)mCallback(nullptr);
	m_activate = false;
	
}

void Goal::BeginContact(b2Contact * contact, bool id)
{
	b2Fixture* fix = id ? contact->GetFixtureA() : contact->GetFixtureB();
	GameObject* obj = static_cast<GameObject*>(fix->GetBody()->GetUserData());
	ObjectType type = obj->getType();
	switch (type) {
		case ObjectType::Player: {
			//mCallback(static_cast<Player*>(obj));
			m_activate = true;
		}break;
	}
}

void Goal::draw(sf::RenderTarget & target, sf::RenderStates states) const {

	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite, states);
}

