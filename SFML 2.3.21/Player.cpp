#include "Player.h"
#include "Constants.h"
#include "Conversions.h"
#include <SFML/Graphics.hpp>
#include "HookCallback.h"
ObjectType Player::getType() const
{
	return ObjectType::Player;
}

Player::Player(const Resources& res, const b2Vec2& position, float mAcceleration, float mAngularAcc, float mRopeLength, float mMaxFuel, float mFuel, float mExplosionImpulse, float mMaxSpeed, bool m_always_accelerating, bool has_fuel, bool has_rope, bool has_steer) :
	mAcceleration(mAcceleration),
	mAngularAcc(mAngularAcc),
	mRopeLength(mRopeLength),
	mMaxFuel(mMaxFuel),
	mFuel(mFuel),
	mMaxSpeed(mMaxSpeed),
	m_accelerating(false),
	m_hooked(false),
	mHook(*this),
	mExplosionImpulse(mExplosionImpulse),
	aimLine(sf::PrimitiveType::Quads, 4),
	m_mira(false),
	m_always_accelerating(m_always_accelerating),
	has_fuel(has_fuel),
	has_rope(has_rope),
	has_steer(has_steer)
{
	mBodyDef.position = position;
	mBodyDef.angle = b2_pi / 2;
	mBodyDef.position = position;
	mBodyDef.allowSleep = false;
	mBodyDef.type = b2BodyType::b2_dynamicBody;
	mBodyDef.angularDamping = 2.0f;
	mBodyDef.linearDamping = 0.1f;
	mBodyDef.userData = this;


	mFixtureDef.friction = 1.0f;
	mFixtureDef.density = 1.6f;
	mFixtureDef.restitution = 0.2f;
	
	//shape definition
	b2PolygonShape* bodyShape(new b2PolygonShape);
	mShape.reset(bodyShape);

	bodyShape->SetAsBox(4.f/3.f * 0.5f, 5.f/16.f * 0.5f);
	

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
Player::Player(const Resources & res, PlayerDefinition * def):
	Player(
		res,
		def->pos,
		def->acceleration,
		def->angular_acceleration,
		def->rope_length,
		def->max_fuel,
		def->fuel,
		def->explosion_impulse,
		def->max_speed,
		def->always_accelerating,
		def->has_fuel,
		def->has_rope,
		def->has_steer
	)
{
}
void Player::initBody(b2World & world){
	//body definition
	GameObject::initBody(world);
	mBody=world.CreateBody(&mBodyDef);
	//mWorld->getC
	//fixture definition	
	//shape definition
	b2PolygonShape Torso;
	b2CircleShape Tip;
	b2CircleShape LeftLeg;
	b2CircleShape RightLeg;

	Torso.SetAsBox(3.f / 4.f * 0.5f, 4.9f / 32.f, b2Vec2(-1.f/32.f, 0), 0.f);
	
	Tip.m_radius = 5.f / 32.f;
	Tip.m_p = b2Vec2(11.f / 32.f, 0.f);

	LeftLeg.m_radius = RightLeg.m_radius = 3.f / 32.f;
	LeftLeg.m_p = b2Vec2(-13.f / 32.f, -6.f / 32.f);
	RightLeg.m_p = b2Vec2(-13.f / 32.f, +6.f / 32.f);
	
	mFixtureDef.shape = &Torso;
	mBody->CreateFixture(&mFixtureDef);
	mFixtureDef.density *= 0.5f;
	mFixtureDef.shape = &Tip;
	mBody->CreateFixture(&mFixtureDef);
	mFixtureDef.shape = &LeftLeg;
	mBody->CreateFixture(&mFixtureDef);
	mFixtureDef.shape = &RightLeg;
	mBody->CreateFixture(&mFixtureDef);

	mHook.init();

}
void Player::setController(const Controller & controller)
{
	mController = &controller;
}
void Player::updateControl(sf::Time dt) { reactToController(*mController,dt); }
void Player::reactToController(const Controller & controller, sf::Time dt)
{
	if (controller.input[Input::Hook]) {
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		auto world_pos_sf = controller.getPositionOnWorld(controller.lastMouseClick);
		//convertir las coordenadas sfml en b2

		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);
		this->throwHookTowardsWorldPosition(world_pos_b2.x, world_pos_b2.y);

	}
	if (controller.input[Input::Hookdown]) {
		this->throwHookTowardsLocalDirection(0.f, -1.f);
	}
	if (controller.input[Input::Mira]) {
		this->setMira(!this->getMira());
		m_mira = this->getMira();
	}
	if (controller.input[Input::Hookup]) {
		this->throwHookTowardsLocalDirection(0.f, 1.f);
	}
	if (controller.input[Input::ReleaseHook]) {
		this->releaseHook();
	}
	if (controller.input[Input::Left])
		this->rotateLeft(dt);
	if (controller.input[Input::Right])
		this->rotateRight(dt);
	if (controller.input[Input::Accelerate])
		this->accelerate(dt);
	else this->decelerate();
	if (controller.input[Input::Die])
		this->explode();
}

bool Player::hasRope() const
{
	return has_rope;
}
bool Player::hasFuel() const
{
	return has_fuel;
}
bool Player::hasSteer() const
{
	return has_steer;
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
	if(has_fuel)
	m_accelerating=true;
}

void Player::decelerate(){
	m_accelerating = m_always_accelerating;
}

void Player::rotateLeft(sf::Time dt){
	if(has_steer)
	mBody->ApplyAngularImpulse(mAngularAcc*dt.asSeconds(),true);
}

void Player::rotateRight(sf::Time dt){
	if (has_steer)
	mBody->ApplyAngularImpulse(-mAngularAcc*dt.asSeconds(),true);
}

void Player::throwHookTowardsWorldPosition(float x, float y){
	//Si ya está enganchado se libera
	if (m_hooked) { return; }
	if (!has_rope) return;
	auto cast_point_global = this->getb2Position();
	auto distance = getDirectionTowards(x, y);
	distance*=mRopeLength;
	auto targetPoint=cast_point_global+distance;
	
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
	b2Vec2 target_point_global(getb2Position()+b2Vec2(x,y));
	
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

void Player::updateAimTowardsWorldPosition(float x, float y)
{
	b2Body* targetBody;
	HookCallback callback(*mBody);
	auto pos = this->getb2Position();
	b2Vec2 direction{ getDirectionTowards(x,y) };
	mWorld->RayCast(&callback, pos, pos + mRopeLength * direction);

	targetBody = callback.getTargetBody();


	sf::Color col;
	if (targetBody) {
		col = sf::Color::Green;
		mAim = callback.getPoint();
	}
	else {
		mAim = pos + mRopeLength*direction;
		col = sf::Color::Red;
	}
	for (int i = 0; i < 4; i++) {

		aimLine[i].color = col;
	}
}

void Player::setMira(bool mira)
{
	this->m_mira = mira;
}

bool Player::getMira()
{
	return m_mira;
}


void Player::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	if(m_hooked){
		mHook.draw(target, states);
	} else if(m_mira)drawAim(target, states);
	states.transform.translate(b2_to_sf_pos(getb2Position()));
	states.transform.rotate(-rad_to_deg(getb2Rotation()));
	if(m_accelerating && mAcceleration>b2_epsilon)target.draw(mFire,states);

	target.draw(mSprite, states);
}

void Player::drawAim(sf::RenderTarget & target, sf::RenderStates states) const{
	sf::Vector2f ppos(b2_to_sf_pos(getb2Position()));
	sf::Vector2f tpos(b2_to_sf_pos(mAim));
	sf::Vector2f distance(tpos - ppos);
	sf::Vector2f versor_normal(distance.y, -distance.x);
	normalize(versor_normal);

	aimLine[0].position = ppos - versor_normal;
	aimLine[1].position = ppos + versor_normal;
	aimLine[2].position = tpos + versor_normal;
	aimLine[3].position = tpos - versor_normal;
	target.draw(aimLine, states);
}

b2Vec2 Player::getDirectionTowards(float x, float y)
{
	//El punto de casteo global
	b2Vec2 cast_point_global{ getb2Position() };

	b2Vec2 targetPoint(x, y);
	b2Vec2 distance(targetPoint - cast_point_global);

	distance.Normalize();
	return distance;
}
