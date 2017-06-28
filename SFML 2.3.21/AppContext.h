#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "GameSettings.h"
class GameStack;
struct AppContext{
	AppContext
		( sf::RenderTarget& screen
		, sf::RenderWindow& window
		, sf::Sprite& displaySprite
		, Resources& resources
		, GameSettings& settings
		)
		: screen(&screen)
		, window(&window)
		, displaySprite(&displaySprite)
		, resources(&resources)
		, settings(&settings)
	{};
	sf::RenderTarget* screen;
	sf::RenderWindow* window;
	sf::Sprite*		  displaySprite;
	Resources*		  resources;
	GameStack*		  stack;
	GameSettings*	  settings;
};