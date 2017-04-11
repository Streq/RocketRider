#pragma once
#include "GameObjectDefinition.h"
struct BlockDefinition : GameObjectDefinition{
	BlockDefinition()
	{
		type = ObjectType::Block;
	}

	inline virtual BlockDefinition* copy() { return new BlockDefinition(*this); }
};

