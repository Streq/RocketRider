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
	size = tilemap->getSize();
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++) {
			//get current pixel
			const auto pos = &pixels[(x + y*size.x) * 4];
			//make color
			sf::Color col(*pos, *(pos + 1), *(pos + 2), *(pos + 3));
			auto def_iterator = defs.find(col.toInteger()|0x000000FF);
			if (def_iterator != defs.end()) {
				GameObjectDefinition* def (def_iterator->second.get()->copy());
				def->pos.x = static_cast<float32>(x);
				def->pos.y = -static_cast<float32>(y);
				mObjects.push_back(GameObjectDefinition::ptr(def));
			}
			//alpha value describes coords in tileset
			mTiles.push_back(~col.a);

		}
	}
}

void Level::loadFromFiles(const std::string & pathtilemap, const std::string & pathconfig)
{
	sf::Image tilemap;
	tilemap.loadFromFile(pathtilemap);
	
	rapidxml::file<> file(&pathconfig[0]);
	
	load(&tilemap,file.data());
}
