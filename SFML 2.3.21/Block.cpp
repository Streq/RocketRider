#include "Block.h"
#include "BlockDefinition.h"
#include "Conversions.h"
Block::Block(const Resources& res, const b2Vec2& size, const b2Vec2& position):
	mSize(size),
	mSprite(res.textures.get(Texture::BLOCK),
		sf::IntRect(0,0,static_cast<int>(size.x*PIXELS_PER_METER),static_cast<int>(size.y*PIXELS_PER_METER))
	)
{
	auto bounds=mSprite.getLocalBounds();
	sf::Vector2f center(bounds.width/2.f,bounds.height/2.f);
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

Block::Block(const Resources & res, BlockDefinition * def): Block(res,def->size, def->pos)
{
}

void Block::initBody(b2World & world){
	//body definition
	
	GameObject::initBody(world);
	
	mBody=world.CreateBody(&mBodyDef);

	
	mBody->CreateFixture(&mFixtureDef);
}

ObjectType Block::getType() const
{
	return ObjectType::Block;
}

void Block::draw(sf::RenderTarget & target, sf::RenderStates states) const{

	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite,states);
}
