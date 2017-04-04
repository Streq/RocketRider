#pragma once
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Controller.h"
#include "GameObject.h"
#include "Player.h"
#include "AppContext.h"

#define rightKey sf::Keyboard::D
#define leftKey sf::Keyboard::A
#define accelerateKey sf::Keyboard::W
class Game{
	public:	
	
	Game(const AppContext& context);
	void handle_event(const sf::Event& e);
	void update(sf::Time dt);
	void draw();
	void init();


	private:
	
	Player::Ptr							mPlayer;
	std::vector<GameObject::Ptr>		mObjects;
	Controller							mController;
	AppContext							mContext;
	b2World								mWorld;
	sf::View							mView;
	sf::Sprite							mBackground;
	std::unique_ptr<b2ContactListener>	mContactListener;
};