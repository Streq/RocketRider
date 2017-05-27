#include "Hook.h"
#include "Conversions.h"
#include "Player.h"
Hook::Hook(Player & mPlayer):
	mPlayer(mPlayer),
	mVec(sf::PrimitiveType::Quads, 4)
{

}

void Hook::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	if (!mActive) {
		
	};

	mVec[0].color = sf::Color::Black;
	mVec[1].color = sf::Color::Black;
	mVec[2].color = sf::Color::Black;
	mVec[3].color = sf::Color::Black;
	//PlayerPosition
	sf::Vector2f ppos(b2_to_sf_pos(mPlayer.getb2Position()));
	//TargetPosition
	sf::Vector2f tpos(b2_to_sf_pos(mJointA->GetAnchorB()));
	sf::Vector2f distance(tpos - ppos);
	sf::Vector2f versor_normal(distance.y, -distance.x);
	normalize(versor_normal);
	
	mVec[0].position = ppos - versor_normal;
	mVec[1].position = ppos + versor_normal;
	mVec[2].position = tpos + versor_normal;
	mVec[3].position = tpos - versor_normal;
	

	target.draw(mVec,states);
}

bool Hook::isActive() const
{
	return mActive;
}

void Hook::activate(b2Body * target, b2Vec2 Local_Point)
{
	mJointDef.bodyB = target;
	mJointDef.localAnchorB = Local_Point;
	b2Vec2 localCenterPlayer = mPlayer.getBody()->GetLocalCenter();
	mJointDef.maxLength = b2Vec2(target->GetWorldPoint(Local_Point) - mPlayer.getBody()->GetWorldCenter()).Length();


	mJointDef.localAnchorA = localCenterPlayer + b2Vec2(0.15f, 0.f);
	mJointA = static_cast<b2RopeJoint*>(mPlayer.getWorld()->CreateJoint(&mJointDef));

	mJointDef.localAnchorA = localCenterPlayer + b2Vec2(-0.15f, 0.f);
	mJointB = static_cast<b2RopeJoint*>(mPlayer.getWorld()->CreateJoint(&mJointDef));

	mActive = true;
}

void Hook::init(){
	mJointDef.bodyA = mPlayer.getBody();
	mJointDef.collideConnected = true;
	mJointDef.type = b2JointType::e_ropeJoint;
}

void Hook::deactivate() {
	mPlayer.getWorld()->DestroyJoint(mJointA);
	mPlayer.getWorld()->DestroyJoint(mJointB);
	mActive = false;
}