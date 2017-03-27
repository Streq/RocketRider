#include "Game.h"
#include "Conversions.h"
#include "Block.h"


Game::Game(const AppContext & context):
	mWorld(b2Vec2(0,-10.f)),
	mContext(context),
	mView(sf::Vector2f(0.f,0.f),sf::Vector2f(INIT_VIEW_SIZE))
{
	

}

void Game::handle_event(const sf::Event & e){
	switch (e.type){
		case sf::Event::KeyReleased:
		case sf::Event::KeyPressed:{
			bool pressed = e.type==sf::Event::KeyPressed;
			switch(e.key.code){
				case leftKey:{
					mController.input[Input::Left]=pressed;
				}break;
				case rightKey:{
					mController.input[Input::Right]=pressed;
				}break;
				case accelerateKey:{
					mController.input[Input::Accelerate]=pressed;
				}break;
			}
		}break;
		case sf::Event::MouseButtonPressed:{
			if(e.mouseButton.button==sf::Mouse::Button::Left){
				mController.input[Input::Hook] = true;
				mController.lastMouseClick = sf::Vector2i(e.mouseButton.x,e.mouseButton.y);
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


	}
}

void Game::update(sf::Time dt){
	if(mController.input[Input::Left])
		mPlayer->rotateLeft(dt);
	if(mController.input[Input::Right])
		mPlayer->rotateRight(dt);
	if(mController.input[Input::Accelerate])
		mPlayer->accelerate(dt);
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
	mView.setCenter(b2_to_sf_pos(mPlayer->getb2Position()));
	const auto& camara_pos = mView.getCenter();
	const auto& background_pos = mBackground.getPosition();
	sf::Vector2i distance((camara_pos-background_pos));
	distance.x -= distance.x%(INIT_VIEW_SIZE.x/2);
	distance.y -= distance.y%(INIT_VIEW_SIZE.y/2);
	mBackground.move(sf::Vector2f(distance));
}

void Game::draw(){
	mContext.window->setView(mView);
	mContext.window->draw(mBackground);
	for(const auto& object:mObjects){
		mContext.window->draw(*object);
	}

	mContext.window->draw(*mPlayer);
	
}

void Game::init(){
	mBackground.setTexture(mContext.resources->textures.get(Texture::SPRITE_TILE));
	mBackground.setTextureRect(sf::IntRect(0, 0, INIT_VIEW_SIZE.x*2, INIT_VIEW_SIZE.y*2));
	auto bounds=mBackground.getLocalBounds();
	mBackground.setOrigin(bounds.width/2.f,bounds.height/2.f);
	//mBackground.setScale(sf::Vector2f(1.f,1.f)*4.f);

	mPlayer.reset(new Player(*mContext.resources));
	mPlayer->initBody(mWorld,b2Vec2(0.f,2.f));
	
	GameObject* floor(new Block(*mContext.resources,b2Vec2(40.f,1.f)));
	floor->initBody(mWorld,b2Vec2(0.f,-4.f));
	mObjects.push_back(std::unique_ptr<GameObject>(floor));


	GameObject* left_wall(new Block(*mContext.resources,b2Vec2(1.f,40.f)));
	left_wall->initBody(mWorld,b2Vec2(-24.f,0.f));
	mObjects.push_back(std::unique_ptr<GameObject>(left_wall));


	GameObject* right_wall(new Block(*mContext.resources,b2Vec2(1.f,40.f)));
	right_wall->initBody(mWorld,b2Vec2(24.f,0.f));
	mObjects.push_back(std::unique_ptr<GameObject>(right_wall));


	GameObject* top_wall(new Block(*mContext.resources,b2Vec2(40.f,1.f)));
	top_wall->initBody(mWorld,b2Vec2(0.f,24.f));
	mObjects.push_back(std::unique_ptr<GameObject>(top_wall));
}
