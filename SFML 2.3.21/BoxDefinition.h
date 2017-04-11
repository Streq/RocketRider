#pragma once
#include "GameObjectDefinition.h"
struct BoxDefinition : GameObjectDefinition {
	BoxDefinition()
	{
		type = ObjectType::Box;
	}

	inline virtual BoxDefinition* copy() { return new BoxDefinition(*this); }
};