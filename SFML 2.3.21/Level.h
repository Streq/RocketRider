#pragma once
#include "GameObject.h"
#include "GameObjectDefinition.h"
#include <vector>
#include "xml_utils.h"
#include <string>
#include "Tilemap.h"
struct Level {
	std::vector<GameObjectDefinition::ptr>	mObjects;
	std::string								start_message;
	std::vector<uint8>						mTiles;
	
	sf::Vector2u							size;


	void load(sf::Image* tilemap, char* configuration);
	void loadFromFiles(const std::string& pathtilemap, const std::string& pathconfig);
};