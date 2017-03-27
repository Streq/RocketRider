#pragma once
#include <SFML/Graphics.hpp>
#include "GameStack.h"
#include "Game.h"
class Application{
	public:
	Application();
	void				run();

	void				handleEvents();
	void				update(sf::Time time);
	void				render();
	private:
	Resources			mResources;
	sf::RenderWindow	mWindow;
	Game				mGame;
	//a futuro
	GameStack			mStack;
	sf::Text			mFPSText;
	
};