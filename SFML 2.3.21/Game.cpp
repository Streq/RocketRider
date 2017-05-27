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
Game::Game(GameStack& s, AppContext context) :
	GameState(s, context),
	mController(std::move(context)),
	mWorld(b2Vec2(0,-10.f)),
	mView(sf::Vector2f(0.f,0.f),sf::Vector2f(static_cast<float>(INIT_VIEW_SIZE),static_cast<float>(INIT_VIEW_SIZE*ASPECT_RATIO))),
	mContactListener(new GameContactListener()),
	mLevels(),
	m_level_index(-1)
{
	
	init();
}



bool Game::handle_event(const sf::Event & e){
	mController.handleEvent(e);
	switch (e.type){
		
		case sf::Event::MouseWheelScrolled:{
			int delta = -sign(e.mouseWheelScroll.delta) * MOUSE_SCROLL_ZOOM;
			//mView.zoom(1.f-delta);
			mView.setSize(mView.getSize() + static_cast<sf::Vector2f>(ASPECT_RATIO_VEC) * static_cast<float>(16 * delta));
			if (mView.getSize().x > MAX_VIEW_SIZE)
				mView.setSize(sf::Vector2f(1.f,ASPECT_RATIO)*(float)MAX_VIEW_SIZE);
			if (mView.getSize().x < MIN_VIEW_SIZE)
				mView.setSize(sf::Vector2f(1.f, ASPECT_RATIO)*(float)MIN_VIEW_SIZE);
			
			
		}
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
	if (mController.input[Input::Hook]) {
		mController.input[Input::Hook] = false;
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		sf::Vector2f world_pos_sf = mContext.window->mapPixelToCoords(mController.lastMouseClick, mView);
		//convertir las coordenadas sfml en b2
		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);
		mPlayer->throwHookTowardsWorldPosition(world_pos_b2.x, world_pos_b2.y);

	}
	if (mController.check_pressed(Input::Hookdown) && mController.check_updated(Input::Hookdown)) {
		mPlayer->throwHookTowardsLocalDirection(0.f, -1.f);
	}
	if (mController.check_pressed(Input::Mira) && mController.check_updated(Input::Mira)) {
		mPlayer->setMira(!mPlayer->getMira());
		m_mira = mPlayer->getMira();
	}
	if (mController.check_pressed(Input::Hookup) && mController.check_updated(Input::Hookup)) {
		mPlayer->throwHookTowardsLocalDirection(0.f, 1.f);
	}
	if (mController.input[Input::ReleaseHook]) {
		mPlayer->releaseHook();
	}
	if (mController.check_pressed(Input::Left))
		mPlayer->rotateLeft(dt);
	if (mController.check_pressed(Input::Right))
		mPlayer->rotateRight(dt);
	if (mController.check_pressed(Input::Accelerate))
		mPlayer->accelerate(dt);
	else mPlayer->decelerate();
	if (mController.check_pressed(Input::Die))
		mPlayer->explode();
	
	mController.clear_updated();

	mWorld.Step(dt.asSeconds(),B2::VELOCITY_ITERATIONS,B2::POSITION_ITERATIONS);
	mPlayer->Step(dt);

	std::for_each(mObjects.begin(), mObjects.end(), [&dt](GameObject::Ptr& ptr) {ptr->Step(dt);});

	if (m_goto_next_level) {
		next_level();
	}
	if (mPlayer->isDead())
		goto_level(m_level_index);

	mView.setCenter(b2_to_sf_pos(mPlayer->getb2Position()));

	{
		//mapear el pixel clickeado en pantalla a las coordenadas del mundo en sfml
		sf::Vector2f world_pos_sf = mContext.window->mapPixelToCoords(mController.lastMousePosition, mView);
		//convertir las coordenadas sfml en b2
		b2Vec2 world_pos_b2 = sf_to_b2_pos(world_pos_sf);

		mPlayer->updateAimTowardsWorldPosition(world_pos_b2.x, world_pos_b2.y);
	}
	//mView.setRotation(-rad_to_deg(mPlayer->getb2Rotation())+90);
	//const auto& camara_pos = mView.getCenter();
	//const auto& background_pos = Stars0.getPosition();
	//sf::Vector2i distance((camara_pos-background_pos));
	//auto bounds = Stars0.getGlobalBounds();
	//distance.x -= sign(distance.x)*abs(distance.x)%(int(bounds.width/3));
	//distance.y -= sign(distance.y)*abs(distance.y)%(int(bounds.height/3));
	//Stars0.move(sf::Vector2f(distance));
	//Stars0.mTexture.adjust_to_view(Stars0.applyDepthToView(mView));
	//Stars1.mTexture.adjust_to_view(Stars1.applyDepthToView(mView));
	//Stars2.mTexture.adjust_to_view(Stars2.applyDepthToView(mView));
	//Stars3.mTexture.adjust_to_view(Stars3.applyDepthToView(mView));
	return false;
}

void Game::draw() const{
	mContext.screen->setView(mView);
	for(const BackgroundLayer& lay : background) {
		mContext.screen->draw(lay); 
	}


	mContext.screen->draw(mTilemap);
	for(const auto& object:mObjects){
		mContext.screen->draw(*object);
	}



	mContext.screen->draw(*mPlayer);
	if(m_display_message){
		mContext.screen->setView(mContext.screen->getDefaultView());
		mContext.screen->draw(mMessage);
	}

}

void Game::init() {
	mController.set_key(InputData(shootupKey, InputData::Type::keyboard), Input::ID::Hookup);
	mController.set_key(InputData(shootdownKey, InputData::Type::keyboard), Input::ID::Hookdown);
	mController.set_key(InputData(leftKey, InputData::Type::keyboard), Input::ID::Left);
	mController.set_key(InputData(accelerateKey, InputData::Type::keyboard), Input::ID::Accelerate);
	mController.set_key(InputData(rightKey, InputData::Type::keyboard), Input::ID::Right);
	mController.set_key(InputData(dieKey, InputData::Type::keyboard), Input::ID::Die);
	mController.set_key(InputData(miraKey, InputData::Type::keyboard), Input::ID::Mira);
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

	
	sf::Text text("", mContext.resources->fonts.get(Font::consola), 20u);
	mMessage.setText(std::move(text));
	mMessage.setLineWidth(40);
	auto view = mContext.screen->getDefaultView();
	mMessage.setPosition(view.getCenter()+sf::Vector2f(0.f,-100.f));
	


	load_levels("Assets/Config/config.xml");
	mWorld.SetContactListener(mContactListener.get());
	next_level();


}

void Game::clear(){
	mObjects.clear();
	mPlayer.reset();
	
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
	mPlayer->initBody(mWorld);
	mPlayer->setMira(m_mira);
	mTilemap.setOrigin(16.f, 16.f);
	mTilemap.load(mContext.resources->textures.get(Texture::TILESET), sf::Vector2u(32u, 32u),&level.mTiles[0],level.size.x,level.size.y);
	

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

				auto* msg = level->first_node("message");
				if(!msg) { throw std::runtime_error("Game config invalid, <message> not found: " + path); }

				const std::string path = "Assets/Maps/";

				auto& lvl = mLevels[i];
				lvl.loadFromFiles(path + map->value(),path + levelconfig->value());
				lvl.start_message = msg->value();
			
			}
			files->remove_node(level);
		}
		
	}
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
