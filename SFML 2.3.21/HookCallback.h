#pragma once
#include "Box2D/Box2D.h"
class HookCallback:public b2RayCastCallback{
	public: 
	HookCallback(b2Body& caster):caster(&caster){};
	
	virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point, 
		const b2Vec2& normal, float32 fraction) final{
		//si el cuerpo es el del jugador no cuenta
		if(fixture->GetBody()==caster)return -1;
		//nos fijamos el tipo del objeto
		switch (static_cast<GameObject*>(fixture->GetBody()->GetUserData())->getType()) {
			case ObjectType::DeathBlock:
			case ObjectType::Goal:
				return -1;
		}
		
		//cuerpo que colisiona con el rayito pasa a ser el actual
		targetBody=fixture->GetBody();
		//punto de colision
		this->point=point;
		//retorna la fracción del largo del rayo, osea que se rehace el raycast pero con este largo ahora
		//eventualmente el raycast va a ser tan corto que no va a chocar con nada 
		//tonces el ultimo que haya sido tocado por el raycast va a ser el mas cercano.
		return fraction;
	};
	b2Body* getTargetBody()const{return targetBody;}
	const b2Vec2& getPoint()const{return point;}
	private:
	b2Body* targetBody=nullptr;
	b2Body* caster=nullptr;
	b2Vec2	point;
};