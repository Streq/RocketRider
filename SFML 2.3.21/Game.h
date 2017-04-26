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
#include "ScreenMessage.h"
#include "TileTexture.h"
#include "Tilemap.h"

#include "BackgroundLayer.h"
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
	void next_level();
	Controller							mController;
	AppContext							mContext;

	ScreenMessage						mMessage;


	std::unique_ptr<b2ContactListener>	mContactListener;
	b2World								mWorld;
	std::vector<GameObject::Ptr>		mObjects;

	Player::Ptr							mPlayer;
	sf::View							mView;
	BackgroundLayer						mBackground0;
	BackgroundLayer						mBackground1;
	BackgroundLayer						mBackground2;
	BackgroundLayer						mBackground3;
	Tilemap								mTilemap;


	std::vector<Level>					mLevels;
	int									m_level_index;
	unsigned							m_level_amount;

	bool								m_goto_next_level;
	bool								m_restart_level;
	bool								m_won;
	sf::Time							m_message_display_time;
	bool								m_display_message;
};