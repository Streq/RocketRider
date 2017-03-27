#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"

struct AppContext{
	AppContext(sf::RenderWindow& window, Resources& resources):window(&window),resources(&resources){};
	AppContext(const AppContext& r): window(r.window), resources(r.resources){};
	AppContext(AppContext&& r): window(std::move(r.window)),resources(std::move(r.resources)){};
	sf::RenderWindow* window;
	Resources*		  resources;
};