#include "Game.h"
#include "Conversions.h"

#include "GameObjectDefinition.h"
#include "PlayerContactListener.h"
#include "PlayerDefinition.h"
#include "BoxDefinition.h"
#include "BlockDefinition.h"
#include "GoalDefinition.h"
#include "Box.h"
#include "Block.h"
#include "Player.h"
#include "Goal.h"
#include "xml_utils.h"
Game::Game(const AppContext & context):
	mWorld(b2Vec2(0,-10.f)),
	mContext(context),
	mView(sf::Vector2f(0.f,0.f),sf::Vector2f(INIT_VIEW_SIZE,INIT_VIEW_SIZE*ASPECT_RATIO)),
	mContactListener(new GameContactListener()),
	mLevels(),
	m_level_index(0)
{
	

}



void Game::handle_event(const sf::Event & e){
	switch (e.type){
		case sf::Event::KeyReleased:
		case sf::Event::KeyPressed:{
			bool pressed = e.type==sf::Event::KeyPressed;
			switch(e.key.code){
				case leftKey:{
					mController.input[Input::Left] = pressed;
				}break;
				case rightKey:{
					mController.input[Input::Right] = pressed;
				}break;
				case accelerateKey:{
					mController.input[Input::Accelerate] = pressed;
				}break;
				case dieKey: {
					mController.input[Input::Die] = pressed;
				}
			}
		}break;
		case sf::Event::MouseButtonPressed:{
			if(e.mouseButton.button==sf::Mouse::Button::Left){
				auto& mWindow = *mContext.window;
				mWindow.setView(mWindow.getDefaultView());
				auto& mSprite = *mContext.displaySprite;
				//Transform to local click
				auto localClick=GlobalToLocalPixel(mWindow,mSprite,sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
				if (mSprite.getLocalBounds().contains(mWindow.mapPixelToCoords(localClick))) {
					mController.input[Input::Hook] = true;

					mController.lastMouseClick = localClick;
					
				}
			}
			if(e.mouseButton.button==sf::Mouse::Button::Right){
				mController.input[Input::ReleaseHook] = true;

			}
		}break;
		case sf::Event::MouseButtonReleased:{
			if(e.mouseButton.button==sf::Mouse::Button::Left){
				mController.input[Input::Hook] = false;
			}
			if(e.mouseButton.button==sf::Mouse::Button::Right){
				mController.input[Input::ReleaseHook] = false;

			}
		}break;
		case sf::Event::MouseWheelScrolled:{
			float delta = e.mouseWheelScroll.delta * MOUSE_SCROLL_ZOOM;
			mView.zoom(1.f-delta);
			

			if (mView.getSize().x > MAX_VIEW_SIZE)
				mView.setSize(sf::Vector2f(1.f,ASPECT_RATIO)*(float)MAX_VIEW_SIZE);
			if (mView.getSize().x < MIN_VIEW_SIZE)
				mView.setSize(sf::Vector2f(1.f, ASPECT_RATIO)*(float)MIN_VIEW_SIZE);
		}


	}
}

void Game::update(sf::Time dt){
	if(mController.input[Input::Left])
		mPlayer->rotateLeft(dt);
	if(mController.input[Input::Right])
		mPlayer->rotateRight(dt);
	if(mController.input[Input::Accelerate])
		mPlayer->accelerate(dt);
	else mPlayer->decelerate();
	if (mController.input[Input::Die]) {
		mPlayer->explode();
	}
	if(mController.input[Input::Hook]){
		//printf("mouse:%d,%d\n",mController.lastMouseClick.x,mController.lastMouseClick.y);
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		sf::Vector2f world_pos_sf = mContext.window->mapPixelToCoords(mController.lastMouseClick,mView);
		//printf("world_sf:%d,%d\n",world_pos_sf.x,world_pos_sf.y);
		//convertir las coordenadas sfml en b2
		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);
		//printf("world_b2:%d,%d\n",world_pos_b2.x,world_pos_b2.y);
		mPlayer->throwHook(world_pos_b2.x, world_pos_b2.y);
		
	}
	if(mController.input[Input::ReleaseHook]){
		mPlayer->releaseHook();
	}
	

	mWorld.Step(dt.asSeconds(),B2::VELOCITY_ITERATIONS,B2::POSITION_ITERATIONS);
	mPlayer->Step();

	std::for_each(mObjects.begin(), mObjects.end(), [](GameObject::Ptr& ptr) {ptr->Step();});

	if (m_goto_next_level) {
		if (++m_level_index < m_level_amount) {
			goto_level(m_level_index);
			m_goto_next_level = false;
		}
		else {
			m_won = true;
		}

		
	}
	if (mPlayer->isDead())
		goto_level(m_level_index);

	mView.setCenter(b2_to_sf_pos(mPlayer->getb2Position()));
	const auto& camara_pos = mView.getCenter();
	const auto& background_pos = mBackground.getPosition();
	sf::Vector2i distance((camara_pos-background_pos));
	auto bounds = mBackground.getGlobalBounds();
	distance.x -= sign(distance.x)*abs(distance.x)%(int(bounds.width/3));
	distance.y -= sign(distance.y)*abs(distance.y)%(int(bounds.height/3));
	mBackground.move(sf::Vector2f(distance));
}

void Game::draw(){
	mContext.screen->setView(mView);
	mContext.screen->draw(mBackground);
	for(const auto& object:mObjects){
		mContext.screen->draw(*object);
	}

	mContext.screen->draw(*mPlayer);
	
}

void Game::init() {
	mBackground.setTexture(mContext.resources->textures.get(Texture::SPRITE_TILE));
	mBackground.setTextureRect(sf::IntRect(0, 0, MAX_VIEW_SIZE * 3, ASPECT_RATIO*MAX_VIEW_SIZE * 3));
	auto bounds = mBackground.getLocalBounds();
	mBackground.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mBackground.setScale(sf::Vector2f(1.f, 1.f)*4.f);

	load_levels("Assets/Config/config.xml");
	mWorld.SetContactListener(mContactListener.get());
	loadLevel(mLevels[0]);


}

void Game::clear(){
	mObjects.clear();
	mPlayer.reset();
	
}

void Game::loadLevel(const Level & level)
{
	for (auto& ptr : level.mObjects) {
		createObject(ptr.get());
		
	}
	for(auto& ptr : mObjects){
		ptr->initBody(mWorld);
	}
	mPlayer->initBody(mWorld);
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
			mPlayer.reset(new Player(*mContext.resources, static_cast<PlayerDefinition*>(def)));
			return;
		}break;
		case ObjectType::Goal: {
			obj = new Goal(*mContext.resources, [this](Player*) {this->m_goto_next_level = true; }, static_cast<GoalDefinition*>(def));
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
	rapidxml::file<>file(&path[0]);
	
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(file.data());

	auto* config = doc.first_node("config");
	if (!config) { throw std::runtime_error("Game config invalid, <config> not found: " + path); }

	auto* levels = config->first_node("levels");

	{
		auto* amount = levels->first_node("amount");
		if (!amount) { throw std::runtime_error("Game config invalid, <amount> not found: " + path); }
		m_level_amount = std::stoul(amount->value());
		mLevels.resize(m_level_amount);
	}
	{
		auto* files = levels->first_node("files");
		if (!files) { throw std::runtime_error("Game config invalid, <files> not found: " + path); }
		for (unsigned i = 0; i < m_level_amount; i++) {
			
			auto* level = files->first_node("level");
			if (!level) { throw std::runtime_error("Game config invalid, <level> not found: " + path); }
			{
				auto* map = level->first_node("map");
				if (!map) { throw std::runtime_error("Game config invalid, <map> not found: " + path); }
				
				auto* levelconfig = level->first_node("levelconfig");
				if (!levelconfig) { throw std::runtime_error("Game config invalid, <levelconfig> not found: " + path); }

				const std::string path = "Assets/Maps/";

				mLevels[i].loadFromFiles(path + map->value(),path + levelconfig->value());

			
			}
			files->remove_node(level);
		}
		
	}
}
