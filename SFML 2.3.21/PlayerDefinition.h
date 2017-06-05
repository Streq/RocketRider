#pragma once
#include "GameObjectDefinition.h"
#include <iostream>

struct PlayerDefinition : public GameObjectDefinition{
	PlayerDefinition() :
		acceleration(30.f),
		angular_acceleration(1.5f),
		rope_length(15.f),
		max_fuel(20.f),
		fuel(20.f),
		max_speed(20.f),
		explosion_impulse(10.f),
		always_accelerating(false),
		has_fuel(true),
		has_rope(true),
		has_steer(true)
	{
		type = ObjectType::Player;
	}

	virtual rapidxml::xml_document<char>* to_xml() {
		auto* doc = GameObjectDefinition::to_xml();
		//xml_document<char>* doc = new xml_document<char>();
		auto* obj = doc->first_node("GameObject");
		
		append_field_node(doc, fuel, obj);
		append_field_node(doc, max_fuel, obj);
		append_field_node(doc, rope_length, obj);
		append_field_node(doc, acceleration, obj);
		append_field_node(doc, max_speed, obj);
		append_field_node(doc, angular_acceleration, obj);
		append_field_node(doc, explosion_impulse, obj);
		append_field_node(doc, always_accelerating, obj);
		append_field_node(doc, has_fuel, obj);
		append_field_node(doc, has_rope, obj);
		append_field_node(doc, has_steer, obj);
		
		return doc;
	}


	inline virtual PlayerDefinition* copy() { return new PlayerDefinition(*this); }


	//virtual void* to_xml();

	float		fuel;
	float		max_fuel;
	float		rope_length;
	float		acceleration;
	float		max_speed;
	float		angular_acceleration;
	float		explosion_impulse;
	bool		always_accelerating;
	bool		has_rope;
	bool		has_fuel;
	bool		has_steer;
};