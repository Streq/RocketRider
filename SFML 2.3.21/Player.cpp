#include "Player.h"
#include "Constants.h"
#include "Conversions.h"
#include <SFML/Graphics.hpp>
#include "HookCallback.h"
GameObject::Type Player::getType() const
{
	return Type::Player;
}

Player::Player(const Resources& res, const b2Vec2& position,float mAcceleration, float mAngularAcc, float mRopeLength, float mMaxFuel, float mFuel, float mExplosionImpulse):
	mAcceleration(mAcceleration),
	mAngularAcc(mAngularAcc),
	mRopeLength(mRopeLength),
	mMaxFuel(mMaxFuel), 
	mFuel(mFuel),
	m_accelerating(false),
	m_hooked(false),
	mHook(*this),
	mExplosionImpulse(mExplosionImpulse)
{
	mBodyDef.position = position;
	mBodyDef.angle = b2_pi / 2;
	mBodyDef.position = position;
	mBodyDef.allowSleep = false;
	mBodyDef.type = b2BodyType::b2_dynamicBody;
	mBodyDef.angularDamping = 0.5f;
	mBodyDef.linearDamping = 0.1f;
	mBodyDef.userData = this;



	mFixtureDef.friction = 1.f;
	mFixtureDef.density = 1.3f;
	mFixtureDef.restitution = 0.2f;
	//shape definition
	b2PolygonShape* bodyShape(new b2PolygonShape);
	mShape.reset(bodyShape);

	bodyShape->SetAsBox(0.5f, 0.25f);


	mFixtureDef.shape = bodyShape;
	
	mSprite.setTexture(res.textures.get(Texture::SPRITE_PLAYER_1));	
	auto bounds=mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width/2.f,bounds.height/2.f);
	
	mFire.setTexture(res.textures.get(Texture::SPRITE_FIRE));	
	bounds=mFire.getLocalBounds();
	mFire.setOrigin(bounds.width/2.f,bounds.height/2.f);
	mFire.setPosition(-bounds.width,0.f);
	
	mExplosionSprite.setTexture(res.textures.get(Texture::SPRITE_EXPLOSION));
	bounds = mExplosionSprite.getLocalBounds();
	mExplosionSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mExplosionSprite.setPosition(0.f, 0.f);
}
void Player::initBody(b2World & world){
	//body definition
	GameObject::initBody(world);
	mBody=world.CreateBody(&mBodyDef);
	//mWorld->getC
	//fixture definition	
	mBody->CreateFixture(&mFixtureDef);
	mHook.init();

}

void Player::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse, bool id){
	if(impulse->normalImpulses[id] > mExplosionImpulse)m_explode = true;
	//printf("%f\n", impulse->normalImpulses[id]);
	//printf("%f\n", impulse->tangentImpulses[id]);
}

void Player::BeginContact(b2Contact * contact, bool id)
{
	GameObject::Type type;
	type = id ? 
		static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData())->getType():
		static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData())->getType();
	if (type == GameObject::Type::Goal)
		m_goal = true;
}

void Player::Step()
{
	if (m_explode)explode();
	m_explode = false;
}

void Player::accelerate(sf::Time dt){
	b2Vec2 vector(rad_to_b2Vec(mBody->GetAngle(), mAcceleration * dt.asSeconds()));
	mBody->ApplyLinearImpulseToCenter(vector,true);
	m_accelerating=true;
}

void Player::decelerate(){
	m_accelerating=false;
}

void Player::rotateLeft(sf::Time dt){
	mBody->ApplyAngularImpulse(mAngularAcc*dt.asSeconds(),true);
}

void Player::rotateRight(sf::Time dt){
	mBody->ApplyAngularImpulse(-mAngularAcc*dt.asSeconds(),true);
}

void Player::throwHook(float x, float y){
	//Si ya está enganchado no hace nada
	if(m_hooked) return;
	b2Vec2 cast_point_local = mBody->GetLocalCenter(); //+ b2Vec2(0.f, 0.7f);
	b2Vec2 cast_point_global(mBody->GetWorldPoint(cast_point_local));
	b2Vec2 targetPoint(x,y);
	b2Vec2 distance(targetPoint-cast_point_global);
	if(distance.LengthSquared() > mRopeLength*mRopeLength){
		distance.Normalize();
		distance*=mRopeLength;
		targetPoint=cast_point_global+distance;
	}

	b2Body* targetBody;
	HookCallback callback(*mBody);
	
	
	mWorld->RayCast(&callback,cast_point_global,targetPoint);
	targetBody=callback.getTargetBody();
	if(targetBody){
		mHook.activate(targetBody, targetBody->GetLocalPoint(callback.getPoint()));
		m_hooked=true;
		
	}

}

void Player::releaseHook(){
	if(m_hooked){
		//mWorld->DestroyJoint(mHook);
		mHook.deactivate();
		m_hooked=false;
	}
}

void Player::explode()
{
	printf("espelote");
}

bool Player::isHooked() const{
	return m_hooked;
}


void Player::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	if(m_hooked){
		mHook.draw(target, states);
		/*
		sf::Vector2f pa(b2_to_sf_pos(mHook->GetAnchorA()));
		sf::Vector2f pb(b2_to_sf_pos(mHook->GetAnchorB()));
		auto line = sf::VertexArray(sf::PrimitiveType::Lines,2);
		line[0].color=Color::Brown;
		line[1].color=Color::Brown;
		line[0].position=pa;
		line[1].position=pb;
		target.draw(line,states);
		*/
	}
	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	if(m_accelerating)target.draw(mFire,states);
	target.draw(mSprite,states);
}
