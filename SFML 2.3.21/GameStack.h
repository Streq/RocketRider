#pragma once
#include "GameState.h"
#include <vector>
#include <map>
#include <functional>
#include "AppContext.h"
#include "Game.h"
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

	template<typename T, typename ...Args>
	void														register_state(GameState::ID id, Args...);


	//template<typename T>
	//inline void													register_state_w_args(GameState::ID id, ...);
	bool														is_empty()const;

	void														update(sf::Time dt);
	void														handle_event(const sf::Event& e);
	void														draw() const;


	void														push_state(GameState::ID);
	void														pop_state();
	void														clear_stack();

	void														apply_changes();
	private:
	std::vector<GameState::Ptr>										mStack;
	std::vector<Request>											mRequests;
	std::map<GameState::ID, std::function<GameState::Ptr()>>		mFactories;
	AppContext														mContext;
};
/*
template<typename T>
inline void GameStack::register_state(GameState::ID id) {
	mFactories[id] = [this]()
	{
		return GameState::Ptr(new T(*this, this->mContext));
	};

}
*/
/*
Lemme see if I get his :)))))
typename ... Args -> let's call this "bunch of types" Args
Args... -> lemme see those types
Args -> nah this just the pack without expanding
args -> pack of INSTANCES of the given types, not types themselves
args... -> EXPANDED DONG of instances
nice okkk thx
*/
template<typename T, typename ...Args>
inline void GameStack::register_state(GameState::ID id,Args... args) {
	mFactories[id] = [this,args...]()//lambda captures args by copy
	{
		return GameState::Ptr(new T(*this, this->mContext, args...));
	};

}
