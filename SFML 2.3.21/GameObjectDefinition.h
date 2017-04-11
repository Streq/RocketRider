#pragma once
#include <Box2D/Box2D.h>

#include "xml_utils.h"

#include "Constants.h"

#include <string>

#include <iostream>

struct GameObjectDefinition {



	using ptr = std::unique_ptr<GameObjectDefinition>;

	ObjectType			type;
	b2Vec2				pos;
	b2Vec2				size;
	
	GameObjectDefinition() :pos(0.f, 0.f), size(1.f, 1.f) {};
	virtual rapidxml::xml_document<char>* to_xml()
	{
		rapidxml::xml_document<char>* doc(new rapidxml::xml_document<>);
		auto* obj = doc->allocate_node(rapidxml::node_element, "GameObject");
		doc->append_node(obj);
		
		//append_field_named_node(doc, static_cast<int>(type), "type", obj);
		
		append_b2_node(doc, pos, "position", obj);
		append_b2_node(doc, size, "size", obj);
		

		return doc;
	}

	inline virtual GameObjectDefinition* copy() = 0;
};
