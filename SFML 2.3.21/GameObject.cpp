#include "GameObject.h"
#include "GameObjectDefinition.h"
#include "Conversions.h"
GameObject::~GameObject()
{
	if (mBody && mWorld)
		mWorld->DestroyBody(mBody);
}



const b2Vec2& GameObject::getb2Position()const{
	return mBody->GetPosition();
}

float GameObject::getb2Rotation()const{
	return mBody->GetAngle();
}

void GameObject::initBody(b2World & world){
	mWorld=&world;
}

b2Body* GameObject::getBody()const{
	return mBody;
}

b2World* GameObject::getWorld()const{
	return mWorld;
}
