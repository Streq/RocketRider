#pragma once
#include "Box2D/Box2D.h"
class HookCallback:public b2RayCastCallback{
	public: 
	HookCallback(b2Body& caster):caster(&caster){};
	virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point, 
		const b2Vec2& normal, float32 fraction){

		if(fixture->GetBody()==caster)return -1;

		targetBody=fixture->GetBody();
		this->point=point;
		return fraction;
	};
	b2Body* getTargetBody()const{return targetBody;}
	const b2Vec2& getPoint()const{return point;}
	private:
	b2Body* targetBody=nullptr;
	b2Body* caster=nullptr;
	b2Vec2	point;
};