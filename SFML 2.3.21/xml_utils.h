#pragma once
#include "Box2D/Box2D.h"
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_iterators.hpp>
#include <SFML/Graphics/Color.hpp>

#define append_field_named_node(ALLOC,FIELD,NAME,APPEND)						\
do{																				\
	auto* NAME_STR = ALLOC->allocate_string(NAME);								\
	auto* STR = ALLOC->allocate_string(std::to_string(FIELD).c_str());			\
	auto* NODE = ALLOC->allocate_node(rapidxml::node_element, NAME_STR, STR);	\
	APPEND->append_node(NODE);													\
}while(false)




#define append_field_node(ALLOC,FIELD,APPEND) append_field_named_node(ALLOC,FIELD,#FIELD,APPEND)


#define append_b2_node(B2_ALLOC,B2_VEC,B2_NAME,B2_APPEND)							\
do{																					\
	auto* B2_STR_NAME = B2_ALLOC->allocate_string(B2_NAME);							\
	auto* B2_NODE = B2_ALLOC->allocate_node(rapidxml::node_element,B2_STR_NAME);	\
	append_field_named_node(B2_ALLOC,B2_VEC.x,"x",B2_NODE);							\
	append_field_named_node(B2_ALLOC,B2_VEC.y,"y",B2_NODE);							\
	B2_APPEND->append_node(B2_NODE);												\
}while(false)


class GameObjectDefinition;
GameObjectDefinition* def_from_xml(rapidxml::xml_node<>* node);

sf::Color parse_color_from_xml(rapidxml::xml_node<>*node);

rapidxml::xml_node<>* color_to_xml(sf::Color col, rapidxml::memory_pool<>* pool);
