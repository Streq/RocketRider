#pragma once
#include "Box2D/Box2D.h"
#include "GameObject.h"
#include "ConsoleOut.h"
class GameContactListener : public b2ContactListener
{
	public:
#define GameObjectA static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData())
#define GameObjectB static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData())
	void BeginContact(b2Contact* contact) override final
	{ /* handle begin event */
		GameObjectA->BeginContact(contact,0);
		GameObjectB->BeginContact(contact,1);
	}
	void EndContact(b2Contact* contact) override final
	{ /* handle end event */
		GameObjectA->EndContact(contact,0);
		GameObjectB->EndContact(contact,1);
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override final
	{
		GameObjectA->PreSolve(contact, oldManifold, 0);
		GameObjectB->PreSolve(contact, oldManifold, 1);
	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override final
	{
		GameObjectA->PostSolve(contact, impulse, 0);
		GameObjectB->PostSolve(contact, impulse, 1);
	
	}
#undef GameObjectA
#undef GameObjectB
};