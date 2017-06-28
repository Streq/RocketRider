#pragma once
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Controller.h"
#include "GameObject.h"
#include "Player.h"
#include "GameObjectDefinition.h"
#include "Level.h"
#include "ScreenMessage.h"
#include "TileTexture.h"
#include "Tilemap.h"
#include "GameState.h"
#include "HUD.h"
#include "BackgroundLayer.h"
#include "ScreenMessenger.h"
#include "GameSettings.h"
#include <functional>
#define rightKey sf::Keyboard::D
#define leftKey sf::Keyboard::A
#define accelerateKey sf::Keyboard::W
#define dieKey sf::Keyboard::R
#define shootupKey sf::Keyboard::F
#define shootdownKey sf::Keyboard::G
#define miraKey sf::Keyboard::M
struct AppContext;
class Game: public GameState{
	public:	
		


	Game(GameStack& s, AppContext context);
	
	virtual bool handle_event(const sf::Event& e) override;
	virtual bool update(sf::Time dt)override;
	virtual void draw()const override;
	
	virtual void init();
	void clear();
	virtual void loadLevel(const Level& level);

	const sf::View& getView();


	protected:
	void createObject(GameObjectDefinition* def);
	
	//these 3 mofos have same length which is the amount of players lmalo¡
	std::vector<Controller>				mControllers;
	std::vector<Player::Ptr>			mPlayers;
	std::vector<sf::View>				mViews;
	std::vector<ScreenMessenger>		mMessengers;

	std::string							mConfigFilePath;

	const  GameSettings*				mSettings;

	std::unique_ptr<b2ContactListener>	mContactListener;
	b2World								mWorld;
	std::vector<GameObject::Ptr>		mObjects;

	HUD									mHUD;
	std::vector<BackgroundLayer>		background;
	Tilemap								mTilemap;
	

	unsigned							m_players_in_map;

	virtual void						handle_win_lose()=0;
	
	//1 Player stuff
	ScreenMessage						mMessage;

	


};

