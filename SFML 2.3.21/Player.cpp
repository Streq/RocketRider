#include "Player.h"
#include "Constants.h"
#include "Conversions.h"
#include <SFML/Graphics.hpp>
#include "HookCallback.h"
Player::Player(const Resources& res,float mAcceleration, float mAngularAcc, float mRopeLength, float mMaxFuel, float mFuel):
	mAcceleration(mAcceleration),
	mAngularAcc(mAngularAcc),
	mRopeLength(mRopeLength),
	mMaxFuel(mMaxFuel), 
	mFuel(mFuel)
{
	mSprite.setTexture(res.textures.get(Texture::SPRITE_PLAYER_3));	
	auto bounds=mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width/2.f,bounds.height/2.f);
	mHooked=false;
}
void Player::initBody(b2World & world, const b2Vec2& position){
	//body definition
	GameObject::initBody(world,position);
	b2BodyDef bodef;
	bodef.angle = b2_pi/2;
	bodef.position = position;
	bodef.allowSleep = false;
	bodef.type=b2BodyType::b2_dynamicBody;
	bodef.angularDamping=0.5f;
	bodef.linearDamping=0.1f;

	mBody=world.CreateBody(&bodef);
	
	//fixture definition
	b2FixtureDef fixdef;
	fixdef.friction = 1.f;
	fixdef.density = 1.3f;
	fixdef.restitution = 0.2f;
	//shape definition
	b2PolygonShape bodyShape;
	bodyShape.SetAsBox(0.5f,0.25f);

	
	fixdef.shape=&bodyShape;
	
	mBody->CreateFixture(&fixdef);

}

void Player::accelerate(sf::Time dt){
	b2Vec2 vector(rad_to_b2Vec(mBody->GetAngle(), mAcceleration * dt.asSeconds()));
	mBody->ApplyLinearImpulseToCenter(vector,true);
}

void Player::rotateLeft(sf::Time dt){
	mBody->ApplyAngularImpulse(mAngularAcc*dt.asSeconds(),true);
}

void Player::rotateRight(sf::Time dt){
	mBody->ApplyAngularImpulse(-mAngularAcc*dt.asSeconds(),true);
}

void Player::throwHook(float x, float y){
	if(mHooked) return;

	b2Vec2 pos(mBody->GetPosition());
	b2Vec2 targetPoint(x,y);
	b2Vec2 distance(targetPoint-pos);
	if(distance.LengthSquared() > mRopeLength*mRopeLength){
		distance.Normalize();
		distance*=mRopeLength;
		targetPoint=pos+distance;
	}

	b2Body* targetBody;
	HookCallback callback(*mBody);
	
	
	mWorld->RayCast(&callback,pos,targetPoint);
	targetBody=callback.getTargetBody();
	if(targetBody){
		mHooked=true;
		b2RopeJointDef jdef;

		jdef.maxLength=mRopeLength;
		jdef.bodyA=mBody;
		jdef.localAnchorA=mBody->GetLocalCenter()+b2Vec2(0.f,0.7f);
		jdef.bodyB=targetBody;
		jdef.localAnchorB=targetBody->GetLocalPoint(callback.getPoint());
		jdef.collideConnected=true;
		
		mHook=(b2RopeJoint*)mWorld->CreateJoint(&jdef);
		
		
	}

}

void Player::releaseHook(){
	if(mHooked){
		mWorld->DestroyJoint(mHook);
		mHooked=false;
	}
}

bool Player::isHooked() const{
	return mHooked;
}


void Player::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	if(mHooked){
		sf::Vector2f pa(b2_to_sf_pos(mHook->GetAnchorA()));
		sf::Vector2f pb(b2_to_sf_pos(mHook->GetAnchorB()));
		auto line = sf::VertexArray(sf::PrimitiveType::Lines,2);
		line[0].color=Color::Brown;
		line[1].color=Color::Brown;
		line[0].position=pa;
		line[1].position=pb;
		target.draw(line,states);
	}
	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	target.draw(mSprite,states);
	
}
