#include "Game.h"
#include "Conversions.h"
#include "ObjectDefinitions.h"
#include "Box.h"
#include "Block.h"
#include "Player.h"
#include "Goal.h"
#include "DeathBlock.h"
#include "PlayerContactListener.h"
#include "xml_utils.h"
#include "TileTexture.h"
Game::Game(GameStack& s, AppContext context, unsigned players) 
	: GameState(s, context)
	, mWorld(b2Vec2(0,-10.f))
	, mViews()
	, mControllers()
	, mPlayers()
	, mContactListener(new GameContactListener())
	, mLevels()
	, m_level_index(-1)
	, m_players_amount(0u)
{
	
	init(2);
}



bool Game::handle_event(const sf::Event & e){

	for(auto& controller : mControllers){
		controller.handleEvent(e);
	}
	switch (e.type){
		case sf::Event::KeyPressed: {
			if(e.key.code==sf::Keyboard::Return)
				requestStackPush(GameState::ID::PAUSE);
		}
	}
	return false;
}

bool Game::update(sf::Time dt){
	m_message_display_time -= dt;

	if (m_message_display_time <= sf::Time::Zero)
		m_display_message = false;
	for(auto& controller: mControllers)
		controller.updateInput();
	for (auto& player : mPlayers)
		player->updateControl(dt);
	float b2time = dt.asSeconds();

	while (b2time > 0.f) {
		mWorld.Step(std::min(B2::TIME_STEP,b2time), B2::VELOCITY_ITERATIONS, B2::POSITION_ITERATIONS);
		b2time -= B2::TIME_STEP;
	}
	for(auto& player : mPlayers)
		player->Step(dt);
	
	std::for_each(mObjects.begin(), mObjects.end(), [&dt](GameObject::Ptr& ptr) {ptr->Step(dt);});

	if (m_goto_next_level) {
		next_level();
	}
	for(auto& player: mPlayers){
		if (player->isDead())
			goto_level(m_level_index);
	}
	mViews[0].setCenter(b2_to_sf_pos(mPlayers[0]->getb2Position()));
	mViews[1].setCenter(b2_to_sf_pos(mPlayers[1]->getb2Position()));

	{
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		sf::Vector2f world_pos_sf = mContext.window->mapPixelToCoords(mControllers[0].lastMousePosition, mViews[0]);
		//convertir las coordenadas sfml en b2
		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);

		mPlayers[0]->updateAimTowardsWorldPosition(world_pos_b2.x, world_pos_b2.y);
	}
	{
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		sf::Vector2f world_pos_sf = mContext.window->mapPixelToCoords(mControllers[1].lastMousePosition, mViews[1]);
		//convertir las coordenadas sfml en b2
		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);

		mPlayers[1]->updateAimTowardsWorldPosition(world_pos_b2.x, world_pos_b2.y);
	}
	mHUD.update(dt);
	return false;
}

const sf::View& Game::getView() { return mViews[0]; }


void Game::draw() const{
	for (auto& view : mViews) {
		mContext.screen->setView(view);
		for (const BackgroundLayer& lay : background) {
			mContext.screen->draw(lay);
		}


		mContext.screen->draw(mTilemap);
		for (const auto& object : mObjects) {
			mContext.screen->draw(*object);
		}


		for (auto& player : mPlayers) {
			mContext.screen->draw(*player);
		}
		if (m_display_message) {
			mContext.screen->setView(mContext.screen->getDefaultView());
			mContext.screen->draw(mMessage);
		}

		mContext.screen->setView(mContext.screen->getDefaultView());
		mContext.screen->draw(mHUD);
	}
	
}

void Game::init(int players) {
	mViews.reserve(players);
	mPlayers.resize(players);
	mControllers.reserve(players);
	if (players == 1) {
		mViews.push_back(sf::View(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE)*1.f, static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO)*1.0f)));
		mViews[0].setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	}
	if (players > 1) {
		mViews.push_back(sf::View(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE)*0.5f, static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO)*0.5f)));
		mViews[0].setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 0.5f));

		mViews.push_back(sf::View(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE)*0.5f, static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO)*0.5f)));
		mViews[1].setViewport(sf::FloatRect(0.5f, 0.f, 0.5f, 0.5f));
		if (players > 2) {
			mViews.push_back(sf::View(
				sf::Vector2f(0.f, 0.f),
				sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE)*0.5f, static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO)*0.5f)));
			mViews[2].setViewport(sf::FloatRect(0.f, 0.5f, 0.5f, 0.5f));
		}
		if (players > 3) {
			mViews.push_back(sf::View(
				sf::Vector2f(0.f, 0.f),
				sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE)*0.5f, static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO)*0.5f)));
			mViews[3].setViewport(sf::FloatRect(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}
	
	mControllers.push_back(Controller(mContext, mViews[0]));
	mControllers.push_back(Controller(mContext, mViews[1]));

	mControllers[0].set_key(InputData(shootupKey, InputData::Type::keyboard), Input::ID::Hookup, true);
	mControllers[0].set_key(InputData(shootdownKey, InputData::Type::keyboard), Input::ID::Hookdown, true);
	mControllers[0].set_key(InputData(leftKey, InputData::Type::keyboard), Input::ID::Left,false);
	mControllers[0].set_key(InputData(accelerateKey, InputData::Type::keyboard), Input::ID::Accelerate, false);
	mControllers[0].set_key(InputData(rightKey, InputData::Type::keyboard), Input::ID::Right, false);
	mControllers[0].set_key(InputData(dieKey, InputData::Type::keyboard), Input::ID::Die, true);
	mControllers[0].set_key(InputData(miraKey, InputData::Type::keyboard), Input::ID::Mira, true);
	mControllers[0].set_key(InputData(sf::Mouse::Button::Left, InputData::Type::mouse), Input::ID::Hook, true);
	mControllers[0].set_key(InputData(sf::Mouse::Button::Right, InputData::Type::mouse), Input::ID::ReleaseHook, true);
	mControllers[0].set_key(InputData(sf::Keyboard::LShift, InputData::Type::keyboard), Input::ID::zoomin, true);
	mControllers[0].set_key(InputData(sf::Keyboard::LControl, InputData::Type::keyboard), Input::ID::zoomout, true);

	mControllers[1].set_key(InputData(sf::Keyboard::Left, InputData::Type::keyboard), Input::ID::Left, false);
	mControllers[1].set_key(InputData(sf::Keyboard::Up, InputData::Type::keyboard), Input::ID::Accelerate, false);
	mControllers[1].set_key(InputData(sf::Keyboard::Right, InputData::Type::keyboard), Input::ID::Right, false);
	mControllers[1].set_key(InputData(sf::Keyboard::RShift, InputData::Type::keyboard), Input::ID::zoomin, true);
	mControllers[1].set_key(InputData(sf::Keyboard::RControl, InputData::Type::keyboard), Input::ID::zoomout, true);



	background.resize(14);
	//Stars0.mTexture.m_sprite.setTexture(mContext.resources->textures.get(Texture::BACKGROUND));

	BackgroundLayer& SeaBlock = background[0];
	BackgroundLayer& Background = background[1];
	BackgroundLayer& Sky = background[2];
	BackgroundLayer& Sea = background[3];
	BackgroundLayer& Ground0 = background[4];
	BackgroundLayer& Ground1 = background[5];
	BackgroundLayer& Stars0 = background[6];
	BackgroundLayer& Stars1 = background[7];
	BackgroundLayer& Stars2 = background[8];
	BackgroundLayer& Stars3 = background[9];
	BackgroundLayer& Clouds0 = background[10];
	BackgroundLayer& Clouds1 = background[11];
	BackgroundLayer& Clouds2 = background[12];
	BackgroundLayer& Clouds3 = background[13];

#define SET_TEXTURE(LAYER,TEXTURE)LAYER.mTexture.setTexture(mContext.resources->textures.get(Texture::##TEXTURE),true)
#define SET_FLAGS(LAYER,LEFT,RIGHT,UP,DOWN)LAYER.mTexture.setFlags((Mosaico::Flags::infiniteUp*UP) | (Mosaico::Flags::infiniteLeft*LEFT) | (Mosaico::Flags::infiniteRight*RIGHT) | (Mosaico::Flags::infiniteDown*DOWN));
	SET_TEXTURE(Stars0, STARS);
	SET_TEXTURE(Stars1, STARS1);
	SET_TEXTURE(Stars2, STARS2);
	SET_TEXTURE(Stars3, STARS1);
	SET_TEXTURE(Background, BACKGROUND);
	SET_TEXTURE(Sky, SKY);
	SET_TEXTURE(Clouds0, CLOUDS);
	SET_TEXTURE(Clouds1, CLOUDS);
	SET_TEXTURE(Clouds2, CLOUDS);
	SET_TEXTURE(Clouds3, CLOUDS);
	SET_TEXTURE(Sea, SEA);
	SET_TEXTURE(Ground0, MOUNTAIN0);
	SET_TEXTURE(Ground1, MOUNTAIN1);
	SET_TEXTURE(SeaBlock, SEA_BLOCK);

	SET_FLAGS(Stars0, true, true, true, false);
	SET_FLAGS(Stars1, true, true, true, false);
	SET_FLAGS(Stars2, true, true, true, false);
	SET_FLAGS(Stars3, true, true, true, false);
	SET_FLAGS(Background, true, true, false, false);
	SET_FLAGS(Sky, true, true, true, false);
	SET_FLAGS(Clouds0, true, true, false, false);
	SET_FLAGS(Clouds1, true, true, false, false);
	SET_FLAGS(Clouds2, true, true, false, false);
	SET_FLAGS(Clouds3, true, true, false, false);
	SET_FLAGS(Sea, true, true, false, false);
	SET_FLAGS(Ground0, true, true, false, false);
	SET_FLAGS(Ground1, true, true, false, false);
	SET_FLAGS(SeaBlock, true, true, false, true);
#undef SET_TEXTURE
#undef SET_FLAGS
	const float size_fact=3.f;

	Stars0.mTexture.setScale(sf::Vector2f(1.f, 1.f)*1.f * size_fact);
	Stars1.mTexture.setScale(sf::Vector2f(1.f, 1.f)*2.f * size_fact);
	Stars2.mTexture.setScale(sf::Vector2f(1.f, 1.f)*3.f * size_fact);
	Stars3.mTexture.setScale(sf::Vector2f(1.f, 1.f)*5.f * size_fact);
	Sky.mTexture.setScale(sf::Vector2f(1.f, 1.f)*1.f * size_fact);
	Background.mTexture.setScale(sf::Vector2f(1.f, 1.f)*1.f * size_fact);
	Clouds0.mTexture.setScale(sf::Vector2f(1.f, 1.f)*0.4f * size_fact*2.f);
	Clouds1.mTexture.setScale(sf::Vector2f(1.f, 1.f)*0.6f * size_fact*2.f);
	Clouds2.mTexture.setScale(sf::Vector2f(1.f, 1.f)*0.8f * size_fact*2.f);
	Clouds3.mTexture.setScale(sf::Vector2f(1.f, 1.f)*1.f * size_fact*2.f);

	Stars0.speed_factor = 0.1f;
	Stars1.speed_factor = 0.2f;
	Stars2.speed_factor = 0.3f;
	Stars3.speed_factor = 0.5f;
	Sky.speed_factor = 0.1f;
	Background.speed_factor = 0.1f;

	Clouds3.speed_factor = 0.8f;
	float speed_factor_fifth = Clouds3.speed_factor * 0.2f;
	Clouds0.speed_factor = 0.8f - speed_factor_fifth * 3;
	Clouds1.speed_factor = 0.8f - speed_factor_fifth * 2;
	Clouds2.speed_factor = 0.8f - speed_factor_fifth;
	
	Clouds0.setPosition(sf::Vector2f(100.f, 1000.f) * Clouds0.speed_factor *size_fact);
	Clouds1.setPosition(sf::Vector2f(0.f, 1000.f) * Clouds1.speed_factor*size_fact);
	Clouds2.setPosition(sf::Vector2f(200.f, 1000.f) * Clouds2.speed_factor*size_fact);
	Clouds3.setPosition(sf::Vector2f(300.f, 1000.f) * Clouds3.speed_factor*size_fact);

	Sea.speed_factor = 0.1f;
	Ground0.speed_factor = 0.2f;
	Ground1.speed_factor = 0.5f;

	Sea.setScale(sf::Vector2f(1.f, 1.f)*1.f*size_fact);
	Ground0.setScale(sf::Vector2f(1.f, 1.f)*2.f*size_fact);
	Ground1.setScale(sf::Vector2f(1.f, 1.f)*5.f*size_fact);

	mHUD.init((*mPlayers[0]),mContext);
	auto screenview = mContext.screen->getDefaultView();
	mHUD.setPosition(screenview.getCenter()+sf::Vector2f(-96.f , screenview.getSize().y/2.f-96.f));
	
	sf::Text text("", mContext.resources->fonts.get(Font::consola), 20u);
	mMessage.setText(std::move(text));
	mMessage.setLineWidth(40);
	auto view = mContext.screen->getDefaultView();
	mMessage.setPosition(view.getCenter()+sf::Vector2f(0.f,-100.f));
	
	m_mira = true;

	load_levels("Assets/Config/config.xml");
	mWorld.SetContactListener(mContactListener.get());
	next_level();


}

void Game::clear(){
	mObjects.clear();
	for(auto& player: mPlayers)
		player.reset();
	m_players_amount = 0;
	
}

void Game::loadLevel(const Level & level)
{
	mMessage.setString(level.start_message);
	for (auto& ptr : level.mObjects) {
		createObject(ptr.get());
		
	}
	for(auto& ptr : mObjects){
		ptr->initBody(mWorld);
	}
	mPlayers[0]->initBody(mWorld);
	mPlayers[0]->setMira(m_mira);
	mPlayers[0]->setController(mControllers[0]);


	mPlayers[1]->initBody(mWorld);
	mPlayers[1]->setMira(m_mira);
	mPlayers[1]->setController(mControllers[1]);

	mTilemap.setOrigin(16.f, 16.f);
	mTilemap.load(mContext.resources->textures.get(Texture::TILESET), sf::Vector2u(32u, 32u),&level.mTiles[0],level.size.x,level.size.y);
	mHUD.setPlayer(*mPlayers[0]);
	

}

void Game::createObject(GameObjectDefinition *def)
{
	GameObject* obj = NULL;
	switch (def->type) {
		case ObjectType::Block: {
			obj = new Block(*mContext.resources, static_cast<BlockDefinition*>(def));
		}break;
		case ObjectType::Box: {
			obj = new Box(*mContext.resources, static_cast<BoxDefinition*>(def));
		}break;
		case ObjectType::Player: {
			
			mPlayers[m_players_amount++].reset(new Player(*mContext.resources, static_cast<PlayerDefinition*>(def)));
			return;
		}break;
		case ObjectType::Goal: {
			obj = new Goal(*mContext.resources, [this](Player*) {this->m_goto_next_level = true; }, static_cast<GoalDefinition*>(def));
		}break;
		case ObjectType::DeathBlock: {
			obj = new DeathBlock(*mContext.resources, static_cast<BlockDefinition*>(def));
		}break;
	}
	mObjects.push_back(std::unique_ptr<GameObject>(obj));


}

void Game::goto_level(unsigned level)
{
	clear();
	loadLevel(mLevels[m_level_index=level]);
}

void Game::load_levels(const std::string& path)
{
	load_levels_from_xml(mLevels, path);
	m_level_amount = mLevels.size();
}

void Game::next_level()
{
	if (++m_level_index < static_cast<int>(m_level_amount)) {
		goto_level(m_level_index);
		m_goto_next_level = false;
		m_display_message = true;
		m_message_display_time = SF::LEVEL_TITLE_TIME;
	}
	else {
		m_won = true;
	}
}
