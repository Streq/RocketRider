#pragma once
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Controller.h"
#include "GameObject.h"
#include "Player.h"
#include "AppContext.h"
#include "GameObjectDefinition.h"
#include "Level.h"


#define rightKey sf::Keyboard::D
#define leftKey sf::Keyboard::A
#define accelerateKey sf::Keyboard::W
#define dieKey sf::Keyboard::R
class Game{
	public:	
	
	Game(const AppContext& context);
	
	void handle_event(const sf::Event& e);
	void update(sf::Time dt);
	void draw();
	void init();
	void clear();
	void loadLevel(const Level& level);



	private:
	void createObject(GameObjectDefinition* def);
	void goto_level(unsigned level);
	void load_levels(const std::string& path);

	Controller							mController;
	AppContext							mContext;

	std::unique_ptr<b2ContactListener>	mContactListener;
	b2World								mWorld;
	std::vector<GameObject::Ptr>		mObjects;

	Player::Ptr							mPlayer;
	sf::View							mView;
	sf::Sprite							mBackground;

	std::vector<Level>					mLevels;
	unsigned							m_level_index;
	unsigned							m_level_amount;

	bool								m_goto_next_level;
	bool								m_restart_level;
	bool								m_won;
};