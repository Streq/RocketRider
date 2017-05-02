#include "Application.h"
#include "Constants.h"
#include <SFML/System/Time.hpp>
#include "MainMenu.h"
Application::Application():
	mWindow(
		sf::VideoMode(INIT_WINDOW_SIZE.x,INIT_WINDOW_SIZE.y),
		"Rocket Rider",
		sf::Style::Default),
	mScreen(),
	mStack(AppContext(mScreen, mWindow, mDisplaySprite, mResources))
{
	mStack.register_state<Game>(GameState::ID::GAME);
	mStack.register_state<MainMenu>(GameState::ID::MAIN_MENU);
	mStack.push_state(GameState::ID::MAIN_MENU);
	
	mScreen.setSmooth(false);
	mScreen.create(INIT_VIEW_SIZE, static_cast<int>(INIT_VIEW_SIZE*ASPECT_RATIO));

	mDisplaySprite.setTexture(mScreen.getTexture());
	mDisplaySprite.setOrigin(sf::Vector2f(mScreen.getSize()) * 0.5f);
	
	mWindow.setKeyRepeatEnabled(false);
	
	mResources.fonts.load(Font::arial,"Assets/Fonts/arial.ttf");
	mResources.fonts.load(Font::consola,"Assets/Fonts/consola.ttf");
	
	mFPSText.setFont(mResources.fonts.get(Font::consola));
	mFPSText.setCharacterSize(15u);
	mResources.textures.load(Texture::PLAYER_1,"Assets/Textures/player.png");
	mResources.textures.load(Texture::PLAYER_2,"Assets/Textures/player2.png");
	mResources.textures.load(Texture::PLAYER_3,"Assets/Textures/player3.png");
	mResources.textures.load(Texture::BLOCK,"Assets/Textures/wall.png");
	mResources.textures.get(Texture::BLOCK).setRepeated(true);
	mResources.textures.load(Texture::TILE, "Assets/Textures/tile.png");
	mResources.textures.get(Texture::TILE).setRepeated(true);
	mResources.textures.load(Texture::BACKGROUND, "Assets/Textures/background.png");
	mResources.textures.get(Texture::BACKGROUND).setRepeated(true);
	mResources.textures.load(Texture::FIRE,"Assets/Textures/fire.png");
	mResources.textures.load(Texture::BOX, "Assets/Textures/box.png");
	mResources.textures.load(Texture::EXPLOSION, "Assets/Textures/explosion.png");
	mResources.textures.load(Texture::GOAL, "Assets/Textures/goal.png");
	mResources.textures.load(Texture::LAVA, "Assets/Textures/lava.png");
	mResources.textures.load(Texture::BUTTON, "Assets/Textures/button.png");
	mResources.textures.load(Texture::TILESET, "Assets/Textures/tileset.png");
	mResources.textures.load(Texture::STARS, "Assets/Textures/stars.png");
	mResources.textures.get(Texture::STARS).setRepeated(true);
	mResources.textures.load(Texture::STARS1, "Assets/Textures/stars1.png");
	mResources.textures.get(Texture::STARS1).setRepeated(true);
	mResources.textures.load(Texture::STARS2, "Assets/Textures/stars2.png");
	mResources.textures.get(Texture::STARS2).setRepeated(true);

	mResources.texts.load(TextFile::MAP_DEF, "Assets/Maps/map.xml");
	mResources.texts.load(TextFile::CONFIG, "Assets/Config/config.xml");
	mResources.texts.load(TextFile::HOWTOPLAY, "Assets/Data/htp.txt");
	mResources.texts.load(TextFile::CREDITS, "Assets/Data/credits.txt");

	
	mStack.apply_changes();


}
void Application::run(){
	//FPS calculations
	sf::Time fps_update_time = sf::Time::Zero;

	unsigned real_frames = 0;
	unsigned ideal_frames = 0;
	bool frame_updated;

	sf::Time elapsed_time = sf::Time::Zero;
	sf::Time delta_time = sf::Time::Zero;
	sf::Clock mClock;
	mClock.restart();
	while (mWindow.isOpen() && !mStack.is_empty()){
		auto clocktime = mClock.restart();
		delta_time += clocktime;
		frame_updated = (delta_time >= SF::TIME_STEP);
		real_frames += frame_updated;
	#ifndef NO_FPS_LIMIT
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

	#endif
	#ifdef NO_FPS_LIMIT
		handleEvents();
		update(clocktime);
		if (delta_time >= SF::FPS_UPDATE_TIME) {
			mFPSText.setString("fps:" + std::to_string(real_frames));
			delta_time -= SF::FPS_UPDATE_TIME;
			real_frames = 0;
		};

		real_frames++;
		render();
	#endif
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
				float current_aspect_ratio = size.y / (float)size.x;
				float xfact = 1.f, yfact = 1.f;
				//greater aspect_ratio means x is the limiting factor
				if (current_aspect_ratio > ASPECT_RATIO) {
					/*
					example:
					ideal = 5:3
					current = 4:7 -> (7/4)/(3/5) = 35/12 -> y is 35/12 times bigger than it should be
					*/
					yfact *=  ASPECT_RATIO / current_aspect_ratio;
					
				}
				//lesser aspect_ratio means y is the limiting factor
				else if (current_aspect_ratio < ASPECT_RATIO) {
					/*
					example:
					ideal = 5:3
					current = 10:3 -> (3/10)/(3/5) = 1/2 -> x is twice as big as it should be
					*/
					xfact *= current_aspect_ratio / ASPECT_RATIO ;
					
				}
				mDisplaySprite.setScale(xfact, yfact);
			}break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:{
				auto mappedPixel = windowToSpriteCoords(mWindow, mDisplaySprite, sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
				if (mDisplaySprite.getLocalBounds().contains(mWindow.mapPixelToCoords(mappedPixel))) {
					e.mouseButton.x = mappedPixel.x;
					e.mouseButton.y = mappedPixel.y;
				}
				else return;
			}break;
			case sf::Event::MouseMoved: {
				auto mappedPixel = windowToSpriteCoords(mWindow, mDisplaySprite, sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
				if (mDisplaySprite.getLocalBounds().contains(mWindow.mapPixelToCoords(mappedPixel))) {
					e.mouseMove.x = mappedPixel.x;
					e.mouseMove.y = mappedPixel.y;
				}
				else return;
			}break;
		}
		mStack.handle_event(e);
	}
}

void Application::update(sf::Time time){
	mStack.update(time);
}

void Application::render(){
	mWindow.clear();
	mScreen.clear();
	mStack.draw();
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
