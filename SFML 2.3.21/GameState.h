#pragma once
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "AppContext.h"
class GameStack;
class GameState{
	public:
	using Ptr = std::unique_ptr<GameState>;
	

	
	enum class ID {
		NONE,
		MAIN_MENU,
		SETTINGS,
		GAME_MODE,
		GAME,
		CHARACTER_SELECT,
		HOW_TO_PLAY,
		CREDITS,
		PAUSE
	};


	GameState(GameStack& s, AppContext c);

	virtual bool				handle_event(const sf::Event& e) = 0;
	virtual bool				update(sf::Time tiempo) = 0;
	virtual void				draw()const = 0;


	protected:
	void						requestStackPush(GameState::ID stateID);
	void						requestStackPop();
	void						requestStackClear();
	AppContext					getContext() const;

	protected:
	AppContext					mContext;
	GameStack*					mStack;


};


