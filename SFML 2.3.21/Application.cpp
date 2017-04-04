#include "Application.h"
#include "Constants.h"
#include <SFML/System/Time.hpp>
Application::Application():
	mWindow(
		sf::VideoMode(INIT_WINDOW_SIZE.x,INIT_WINDOW_SIZE.y),
		"Rocket Rider",
		sf::Style::Default),
	mScreen(),
	mGame(AppContext(mScreen, mWindow,mDisplaySprite, mResources))
{
	mScreen.setSmooth(false);
	mScreen.create(INIT_VIEW_SIZE.x, INIT_VIEW_SIZE.y);

	mDisplaySprite.setTexture(mScreen.getTexture());
	mDisplaySprite.setOrigin(sf::Vector2f(mScreen.getSize()) * 0.5f);
	
	mWindow.setKeyRepeatEnabled(false);
	
	mResources.fonts.load(Font::arial,"Assets/Fonts/arial.ttf");
	mResources.fonts.load(Font::consola,"Assets/Fonts/consola.ttf");
	mFPSText.setFont(mResources.fonts.get(Font::consola));
	
	mResources.textures.load(Texture::SPRITE_PLAYER_1,"Assets/Textures/player.png");
	mResources.textures.load(Texture::SPRITE_PLAYER_2,"Assets/Textures/player2.png");
	mResources.textures.load(Texture::SPRITE_PLAYER_3,"Assets/Textures/player3.png");
	mResources.textures.load(Texture::SPRITE_BLOCK,"Assets/Textures/wall.png");
	mResources.textures.get(Texture::SPRITE_BLOCK).setRepeated(true);
	mResources.textures.load(Texture::SPRITE_TILE,"Assets/Textures/tile.png");
	mResources.textures.get(Texture::SPRITE_TILE).setRepeated(true);
	mResources.textures.load(Texture::SPRITE_FIRE,"Assets/Textures/fire.png");
	mResources.textures.load(Texture::SPRITE_BOX, "Assets/Textures/box.png");
	mResources.textures.load(Texture::SPRITE_EXPLOSION, "Assets/Textures/explosion.png");
	mResources.textures.load(Texture::SPRITE_GOAL, "Assets/Textures/goal.png");
	mGame.init();
}
void Application::run(){
	//Time
	sf::Time elapsed_time = sf::Time::Zero;
	sf::Clock clock;
	//FPS calculations
	sf::Time fps_update_time = sf::Time::Zero;
	unsigned	frames;
	bool		frame;

	while(mWindow.isOpen()){
		unsigned real_frames = 0;
		unsigned ideal_frames = 0;
		bool frame_updated;

		sf::Time elapsed_time = sf::Time::Zero;
		sf::Time delta_time = sf::Time::Zero;
		sf::Clock mClock;
		mClock.restart();
		while (mWindow.isOpen()){
			delta_time += mClock.restart();
			frame_updated = (delta_time >= SF::TIME_STEP);
			real_frames += frame_updated;

			while(delta_time >= SF::TIME_STEP){
				ideal_frames++;
				if(ideal_frames == FPS){
					mFPSText.setString("fps:" + std::to_string(real_frames));
					ideal_frames = 0;
					real_frames = 0;
				}
				delta_time -= SF::TIME_STEP;
				elapsed_time += SF::TIME_STEP;
				handleEvents();

				update(SF::TIME_STEP);

				
			}
			if(frame_updated){
				render();
			}
		}
		
	}
}

void Application::handleEvents(){
	sf::Event e;
	while(mWindow.pollEvent(e)){
		switch(e.type){
			case sf::Event::Closed:
				mWindow.close();
				break;
			case sf::Event::Resized:{
				//get new window size
				sf::Vector2u size=mWindow.getSize();
				//get limiting factor
				float current_aspect_ratio = size.x / (float)size.y;
				float xfact = 1.f, yfact = 1.f;
				//greater aspect_ratio means y is the limiting factor
				if (current_aspect_ratio > ASPECT_RATIO) {
					/*
					example:
					ideal = 5/3
					current = 10/3 -> (10/3)/(5/3) = 2/1 -> x is twice as big as it should be
					*/
					xfact *= ASPECT_RATIO / current_aspect_ratio ;
					
				}
				//lesser aspect_ratio means x is the limiting factor
				else if (current_aspect_ratio < ASPECT_RATIO) {
					/*
					example:
					ideal = 5/3
					current = 4/7 -> (4/7)/(5/3) = 12/35 -> y is 35/12 times bigger than it should be
					*/
					yfact *= current_aspect_ratio / ASPECT_RATIO;
					
				}
				mDisplaySprite.setScale(xfact, yfact);
			}
			break;
			

		}
		mGame.handle_event(e);
	}
}

void Application::update(sf::Time time){
	mGame.update(time);
}

void Application::render(){
	mWindow.clear();
	mScreen.clear();
	mGame.draw();
	mScreen.setView(mScreen.getDefaultView());
	mScreen.draw(mFPSText);
	mScreen.display();

	//center the sprite
	sf::Vector2f screen_center (mScreen.getSize());
	screen_center *= 0.5f;

	mDisplaySprite.setPosition(screen_center);

	mWindow.draw(mDisplaySprite);
	mWindow.display();
}
