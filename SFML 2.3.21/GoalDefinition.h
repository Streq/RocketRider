#pragma once
#include "GameObjectDefinition.h"
struct GoalDefinition : GameObjectDefinition {
	GoalDefinition()
	{
		type = ObjectType::Goal;
	}

	inline virtual GoalDefinition* copy() { return new GoalDefinition(*this); }
};