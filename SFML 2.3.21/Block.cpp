#include "Block.h"
#include "Conversions.h"
Block::Block(const Resources& res, const b2Vec2& size):
	mSize(size),
	mSprite(res.textures.get(Texture::SPRITE_BLOCK),
		sf::IntRect(0,0,size.x*PIXELS_PER_METER,size.y*PIXELS_PER_METER)
	)
{
	auto bounds=mSprite.getLocalBounds();
	sf::Vector2f center(bounds.width/2.f,bounds.height/2.f);
	mSprite.setOrigin(center);
}

void Block::initBody(b2World & world, const b2Vec2 & position){
	//body definition
	b2BodyDef bodef;

	GameObject::initBody(world,position);
	bodef.position = position;
	bodef.type=b2BodyType::b2_staticBody;
	

	mBody=world.CreateBody(&bodef);

	//fixture definition
	b2FixtureDef fixdef;
	fixdef.friction = 1.f;
	//shape definition
	b2PolygonShape bodyShape;
	bodyShape.SetAsBox(mSize.x/2.f, mSize.y/2.f);


	fixdef.shape=&bodyShape;

	mBody->CreateFixture(&fixdef);
}

void Block::draw(sf::RenderTarget & target, sf::RenderStates states) const{

	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite,states);
}
