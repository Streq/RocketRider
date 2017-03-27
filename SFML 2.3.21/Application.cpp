#include "Application.h"
#include "Constants.h"
#include <SFML/System/Time.hpp>
Application::Application():
	mWindow(
		sf::VideoMode(INIT_WINDOW_SIZE.x,INIT_WINDOW_SIZE.y),
		"Rocket Rider",
		sf::Style::Default),
	mGame(AppContext(mWindow,mResources))
{
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
			case sf::Event::Resized:
				sf::Vector2u size=mWindow.getSize();

				auto desk=sf::VideoMode::getDesktopMode();
				sf::Vector2u screensize(desk.width,desk.height);
				//get max_factor
				unsigned max_x_factor = screensize.x/ASPECT_RATIO.x;
				unsigned max_y_factor = screensize.y/ASPECT_RATIO.y;
				unsigned max_factor = max_x_factor<max_y_factor?max_x_factor:max_y_factor;
				//get current factor
				unsigned current_factor = sqrt(size.x*size.y/(ASPECT_RATIO.x*ASPECT_RATIO.y));
				if(current_factor>max_factor)current_factor=max_factor;
				mWindow.setSize(ASPECT_RATIO*current_factor);
				
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
	mGame.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mFPSText);
	mWindow.display();
}
