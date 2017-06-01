#include "xml_utils.h"
#include "ObjectDefinitions.h"
#include "Constants.h"
#include "Conversions.h"
#include "Level.h"
#include <ios>
GameObjectDefinition * def_from_xml(rapidxml::xml_node<>* node)
{
	GameObjectDefinition* obj = NULL;
	if (std::strcmp(node->name(), "GameObject"))return obj;
	auto* type_node = (node->first_node("type"));
	if (!type_node)return obj;
	//ObjectType type = static_cast<ObjectType>(std::stoi(type_node->value()));
	auto type = type_from_string(type_node->value());
	switch (type) {
		case ObjectType::Block: {
			obj = new BlockDefinition;
		}break;
		case ObjectType::DeathBlock: {
			obj = new BlockDefinition;
			obj->type = ObjectType::DeathBlock;
		}break;
		case ObjectType::Player: {
			PlayerDefinition* play = new PlayerDefinition;
			
			auto* fuel_node = node->first_node("fuel");
			if (fuel_node) {
				play->fuel = std::stof(fuel_node->value()); 
			}
			auto* max_fuel_node = node->first_node("max_fuel");
			if (max_fuel_node) {
				play->max_fuel = std::stof(max_fuel_node->value());
			}
			auto* rope_length_node = node->first_node("rope_length");
			if (rope_length_node) { 
				play->rope_length = std::stof(rope_length_node->value());
			}
			auto* max_speed_node = node->first_node("max_speed");
			if (max_speed_node) { 
				play->max_speed = std::stof(max_speed_node->value());
			}
			auto* angular_acceleration_node = node->first_node("angular_acceleration");
			if (angular_acceleration_node) { 
				play->angular_acceleration = std::stof(angular_acceleration_node->value()); 
			}
			auto* explosion_impulse_node = node->first_node("explosion_impulse");
			if (explosion_impulse_node) {
				play->explosion_impulse = std::stof(explosion_impulse_node->value()); 
			}
			auto* acceleration_node = node->first_node("acceleration");
			if (acceleration_node) { 
				play->acceleration = std::stof(acceleration_node->value()); 
			}
			
			auto* always_accelerating_node = node->first_node("always_accelerating");
			if (always_accelerating_node) {
				std::istringstream(always_accelerating_node->value()) >> std::boolalpha >> play->always_accelerating;
			}
			auto* has_fuel_node = node->first_node("has_fuel");
			if (has_fuel_node) {
				std::istringstream(has_fuel_node->value()) >> std::boolalpha >> play->has_fuel;
			}
			auto* has_rope_node = node->first_node("has_rope");
			if (has_rope_node) {
				std::istringstream(has_rope_node->value()) >> std::boolalpha >> play->has_rope;
			}
			auto* has_steer_node = node->first_node("has_steer");
			if (has_steer_node) {
				std::istringstream(has_steer_node->value()) >> std::boolalpha >> play->has_steer;
			}


			obj = play;


		}break;
		case ObjectType::Goal: {
			obj = new GoalDefinition;
		}break;
		case ObjectType::Box: {
			obj = new BoxDefinition;
		}break;
	}
	auto* pos_node = node->first_node("position");
	if (pos_node) {
		obj->pos.x = std::stof(pos_node->first_node("x")->value());
		obj->pos.y = std::stof(pos_node->first_node("y")->value());
	}
	auto* size_node = node->first_node("size");
	if (size_node) {
		obj->size.x = std::stof(size_node->first_node("x")->value());
		obj->size.y = std::stof(size_node->first_node("y")->value());
	}
	return obj;
}

sf::Color parse_color_from_xml(rapidxml::xml_node<>* node, bool maxed_alpha)
{
	//ej <color>FFFFFF</color>

	//ej 
	//<color>
	//	<code>FFFFFF</code>
	//	<alpha>255</alpha>
	//</color>
	//std::cout << *node << '\n';
	auto color_node = node->first_node("color");
	auto code = color_node->first_node("code");
	auto alpha = color_node->first_node("alpha");
	const unsigned long code_val = code ? std::stoul(code->value(), NULL, 16) << 8 : 0;
	const unsigned long alpha_val = (alpha && !maxed_alpha) ? std::stoul(alpha->value(), NULL, 10) : 255;
	/*
	std::cout << std::hex << col << '\n';
	std::cout << std::dec;
	std::cout << "Red:" << (unsigned)color.r << '\n';
	std::cout << "Green:" << (unsigned)color.g << '\n';
	std::cout << "Blue:" << (unsigned)color.b << '\n';
	std::cout << "Alpha:" << (unsigned)color.a << '\n';
	*/
	sf::Uint32 col = code_val | alpha_val;

	
	return sf::Color(col);
}

rapidxml::xml_node<>* color_to_xml(sf::Color col, rapidxml::memory_pool<>* pool, bool include_alpha)
{
	const auto str = pool->allocate_string("color");
	const auto code = pool->allocate_string("code");
	const auto code_val = pool->allocate_string(u32_to_hex_string(col.toInteger() >> 8).c_str());
	auto* codenode = pool->allocate_node(rapidxml::node_element, code, code_val);
	auto* colornode = pool->allocate_node(rapidxml::node_element, str);
	colornode->append_node(codenode);
	if(include_alpha){
		const auto alpha = pool->allocate_string("alpha");
		const auto alpha_val = pool->allocate_string(std::to_string(col.a).c_str());
		auto* alphanode = pool->allocate_node(rapidxml::node_element, alpha, alpha_val);
		colornode->append_node(alphanode);
	}
	return colornode;
}

void load_levels_from_xml(std::vector<Level>& mLevels, std::string path)
{
	rapidxml::file<>file(&path[0]);

	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(file.data());

	auto* config = doc.first_node("config");
	if (!config) { throw std::runtime_error("Game config invalid, <config> not found: " + path); }

	auto* levels = config->first_node("levels");
	unsigned m_level_amount;
	{
		auto* amount = levels->first_node("amount");
		if (!amount) { throw std::runtime_error("Game config invalid, <amount> not found: " + path); }
		m_level_amount = std::stoul(amount->value());
		mLevels.resize(m_level_amount);
	}
	{
		auto* files = levels->first_node("files");
		if (!files) { throw std::runtime_error("Game config invalid, <files> not found: " + path); }
		for (unsigned i = 0; i < m_level_amount; i++) {

			auto* level = files->first_node("level");
			if (!level) { throw std::runtime_error("Game config invalid, <level> not found: " + path); }
			{
				auto* map = level->first_node("map");
				if (!map) { throw std::runtime_error("Game config invalid, <map> not found: " + path); }
				//level config contains equivalences between pixels and objects, and the handicaps of the player
				auto* levelconfig = level->first_node("levelconfig");
				if (!levelconfig) { throw std::runtime_error("Game config invalid, <levelconfig> not found: " + path); }

				auto* msg = level->first_node("message");
				if (!msg) { throw std::runtime_error("Game config invalid, <message> not found: " + path); }

				const std::string path = "Assets/Maps/";

				auto& lvl = mLevels[i];
				lvl.loadFromFiles(path + map->value(), path + levelconfig->value());
				lvl.start_message = msg->value();

			}
			files->remove_node(level);
		}

	}
}

