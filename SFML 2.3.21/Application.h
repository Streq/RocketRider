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
	sf::RenderTexture	mScreen;
	sf::Sprite			mDisplaySprite;
	float				mAspectRatio;

	GameStack			mStack;
	GameSettings		mSettings;
	sf::Text			mFPSText;
	
};