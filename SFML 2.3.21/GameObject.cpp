#include "GameObject.h"

const b2Vec2& GameObject::getb2Position()const{
	return mBody->GetPosition();
}

float GameObject::getb2Rotation()const{
	return mBody->GetAngle();
}

void GameObject::initBody(b2World & world, const b2Vec2 & position){
	mWorld=&world;
}
