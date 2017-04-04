#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"

struct AppContext{
	AppContext(sf::RenderTarget& screen, sf::RenderWindow& window, sf::Sprite& displaySprite, Resources& resources):screen(&screen),window(&window),displaySprite(&displaySprite), resources(&resources){};
	//AppContext(const AppContext& r): screen(r.screen), resources(r.resources){};
	//AppContext(AppContext&& r): screen(std::move(r.screen)),resources(std::move(r.resources)){};
	sf::RenderTarget* screen;
	sf::RenderWindow* window;
	sf::Sprite*		  displaySprite;
	Resources*		  resources;
};