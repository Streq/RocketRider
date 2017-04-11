#include "GameStack.h"

GameStack::Request::Request(Action action, GameState::ID state) :
	action(action),
	state(state)
{}

GameStack::GameStack(AppContext mContext) :
	mContext(mContext)
{}

bool GameStack::is_empty() const {
	return mStack.empty();
}

void GameStack::update(sf::Time dt) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++) {
		if (!(*itr)->update(dt)) break;
	}
	apply_changes();
}

void GameStack::handle_event(const sf::Event & e) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++) {
		if (!(*itr)->handle_event(e)) break;
	}
}

void GameStack::draw() const {
	for (auto itr = mStack.begin(); itr != mStack.end(); itr++) {
		(*itr)->draw();
	}
}

void GameStack::push_state(GameState::ID state) {
	mRequests.push_back(Request(Request::Push, state));
}

void GameStack::pop_state() {
	mRequests.push_back(Request(Request::Pop));
}

void GameStack::clear_stack() {
	mRequests.push_back(Request(Request::Clear));
}

void GameStack::apply_changes() {
	for (auto itr = mRequests.begin(); itr != mRequests.end(); itr++) {
		switch (itr->action) {
			case Request::Push:
				mStack.push_back(mFactories[itr->state]());
				break;

			case Request::Pop:
				mStack.pop_back();
				break;

			case Request::Clear:
				mStack.clear();
				break;
		}
	}
	mRequests.clear();
}

