#include "Level.h"
#include <map>
#include "Conversions.h"
void Level::load(sf::Image * tilemap, char* configuration)
{
	//Load the definitions map
	std::map<sf::Uint32, GameObjectDefinition::ptr> defs;
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(configuration);
	auto config = doc.first_node("config");

	auto equivalence = config->first_node("equivalence");
	while(equivalence){
		auto color = parse_color_from_xml(equivalence);
		auto def = def_from_xml(equivalence->first_node("GameObject"));
		defs.insert(std::pair<sf::Uint32, GameObjectDefinition::ptr>(color.toInteger(), def));
		equivalence = equivalence->next_sibling("equivalence");
	}

	//Load the map itself
	auto pixels=tilemap->getPixelsPtr();
	const auto size = tilemap->getSize();
	for (size_t y = 0; y < size.y; y++) {
		for (size_t x = 0; x < size.x; x++) {
			const auto pos = &pixels[(x + y*size.x) * 4];
			auto it = defs.find(sf::Color(*pos, *(pos + 1), *(pos + 2), *(pos + 3)).toInteger());
			if (it != defs.end()) {
				GameObjectDefinition* def (it->second.get()->copy());
				def->pos.x = x;
				def->pos.y = -(int)y;
				mObjects.push_back(GameObjectDefinition::ptr(def));
			}
		}
	}
}
