#pragma once
#include "GameState.h"
#include <vector>
#include <map>
#include <functional>
#include "AppContext.h"
class GameStack {
	public:
	struct Request {

		enum Action {
			Push,
			Pop,
			Clear,
		};

		Action					action;
		GameState::ID			state;

		Request(Action action, GameState::ID state = GameState::ID::NONE);
	};

	GameStack(AppContext mContext);

	template<typename T>
	void														register_state(GameState::ID id);
	bool														is_empty()const;

	void														update(sf::Time dt);
	void														handle_event(const sf::Event& e);
	void														draw() const;


	void														push_state(GameState::ID);
	void														pop_state();
	void														clear_stack();

	void														apply_changes();
	private:
	std::vector<GameState::Ptr>									mStack;
	std::vector<Request>										mRequests;
	std::map<GameState::ID, std::function<GameState::Ptr()>>	mFactories;
	AppContext													mContext;
};

template<typename T>
inline void GameStack::register_state(GameState::ID id) {
	mFactories[id] = [this]()
	{
		return GameState::Ptr(new T(*this, this->mContext));
	};

}
