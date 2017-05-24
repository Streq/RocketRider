#include "Player.h"
#include "Constants.h"
#include "Conversions.h"
#include <SFML/Graphics.hpp>
#include "HookCallback.h"
ObjectType Player::getType() const
{
	return ObjectType::Player;
}

Player::Player(const Resources& res, const b2Vec2& position,float mAcceleration, float mAngularAcc, float mRopeLength, float mMaxFuel, float mFuel, float mExplosionImpulse, float mMaxSpeed, bool m_always_accelerating):
	mAcceleration(mAcceleration),
	mAngularAcc(mAngularAcc),
	mRopeLength(mRopeLength),
	mMaxFuel(mMaxFuel), 
	mFuel(mFuel),
	mMaxSpeed(mMaxSpeed),
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
	mBodyDef.angularDamping = 1.5f;
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
	
	mSprite.setTexture(res.textures.get(Texture::PLAYER_1));	
	auto bounds=mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width/2.f,bounds.height/2.f);
	
	mFire.setTexture(res.textures.get(Texture::FIRE));	
	bounds=mFire.getLocalBounds();
	mFire.setOrigin(bounds.width/2.f,bounds.height/2.f);
	mFire.setPosition(-bounds.width,0.f);
	
	mExplosionSprite.setTexture(res.textures.get(Texture::EXPLOSION));
	bounds = mExplosionSprite.getLocalBounds();
	mExplosionSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mExplosionSprite.setPosition(0.f, 0.f);
}
Player::Player(const Resources & res, PlayerDefinition * def):Player(
	res,
	def->pos,
	def->acceleration,
	def->angular_acceleration,
	def->rope_length,
	def->max_fuel,
	def->fuel,
	def->explosion_impulse,
	def->max_speed,
	def->always_accelerating
)
{
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
	

	int32 count = contact->GetManifold()->pointCount;

	float32 maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}
	if (maxImpulse > mExplosionImpulse)m_explode = true;

}

void Player::BeginContact(b2Contact * contact, bool id)
{
	
	b2Fixture* fix=id? contact->GetFixtureA(): contact->GetFixtureB();
	ObjectType type = static_cast<GameObject*>(fix->GetBody()->GetUserData())->getType();
	
	//if (type == ObjectType::Goal)
	//	m_goal = true;
}

void Player::Step(sf::Time dt)
{
	if(m_accelerating){
		b2Vec2 vector(rad_to_b2Vec(mBody->GetAngle(), mAcceleration * dt.asSeconds()));
		mBody->ApplyLinearImpulseToCenter(vector, true);
	}
	if (m_explode)
		explode();
	m_explode = false;
	
}

void Player::accelerate(sf::Time dt){
	m_accelerating=true;
}

void Player::decelerate(){
		m_accelerating = m_always_accelerating;
}

void Player::rotateLeft(sf::Time dt){
	mBody->ApplyAngularImpulse(mAngularAcc*dt.asSeconds(),true);
}

void Player::rotateRight(sf::Time dt){
	mBody->ApplyAngularImpulse(-mAngularAcc*dt.asSeconds(),true);
}

void Player::throwHookTowardsWorldPosition(float x, float y){
	//Si ya está enganchado primero se libera
	if (m_hooked) { releaseHook(); return; }
	//El punto de casteo relativo al jugador
	b2Vec2 cast_point_local = mBody->GetLocalCenter(); //+ b2Vec2(0.f, 0.7f);
	//El punto de casteo global
	b2Vec2 cast_point_global(mBody->GetWorldPoint(cast_point_local));
	
	b2Vec2 targetPoint(x,y);
	b2Vec2 distance(targetPoint-cast_point_global);
	
	distance.Normalize();
	distance*=mRopeLength;
	targetPoint=cast_point_global+distance;
	
	b2Body* targetBody;
	HookCallback callback(*mBody);
	
	
	mWorld->RayCast(&callback,cast_point_global,targetPoint);
	targetBody=callback.getTargetBody();
	if(targetBody){
		mHook.activate(targetBody, targetBody->GetLocalPoint(callback.getPoint()));
		m_hooked=true;
		
	}

}

void Player::throwHookTowardsLocalDirection(float x, float y) {
	//El punto de origen del casteo relativo al jugador
	b2Vec2 cast_point_local = mBody->GetLocalCenter(); 
	b2Vec2 target_point_global(mBody->GetWorldPoint(cast_point_local+b2Vec2(x,y)));
	
	throwHookTowardsWorldPosition(target_point_global.x,target_point_global.y);
	
}


void Player::releaseHook(){
	if(m_hooked){
		this->mBody->SetAngularVelocity(0.f);
		//mWorld->DestroyJoint(mHook);
		mHook.deactivate();
		m_hooked=false;
	}
}

void Player::explode()
{
	m_dead = true;
}

void Player::setGoalCompleted(bool flag)
{
	m_goal = flag;
}

bool Player::goalCompleted()
{
	return m_goal;
}

bool Player::isDead()const
{
	return m_dead;
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
	if(m_accelerating&&mAcceleration>b2_epsilon)target.draw(mFire,states);
	target.draw(mSprite,states);
}
