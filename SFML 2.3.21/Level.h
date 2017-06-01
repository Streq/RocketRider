#pragma once
#include "GameObjectDefinition.h"
#include <vector>
#include <string>
#include <SFML/Graphics/Image.hpp>
struct Level {
	std::vector<GameObjectDefinition::ptr>	mObjects;
	std::string								start_message;
	//vector of pixels
	std::vector<uint8>						mTiles;
	//dimensions of the map
	sf::Vector2u							size;


	void load(sf::Image* tilemap, char* configuration);
	void loadFromFiles(const std::string& pathtilemap, const std::string& pathconfig);
};