#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
class GameStack;
struct AppContext{
	AppContext(sf::RenderTarget& screen, sf::RenderWindow& window, sf::Sprite& displaySprite, Resources& resources):screen(&screen),window(&window),displaySprite(&displaySprite), resources(&resources){};
	sf::RenderTarget* screen;
	sf::RenderWindow* window;
	sf::Sprite*		  displaySprite;
	Resources*		  resources;
	GameStack*		  stack;
};