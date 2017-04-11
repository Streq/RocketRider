#pragma once
#include "GameObject.h"
#include "GameObjectDefinition.h"
#include <vector>
#include "xml_utils.h"
#include <string>
struct Level {
	std::vector<GameObjectDefinition::ptr> mObjects;

	void load(sf::Image* tilemap, char* configuration);
};