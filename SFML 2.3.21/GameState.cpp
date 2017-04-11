#include "GameState.h"
#include "GameStack.h"
GameState::GameState(GameStack& s, AppContext c) :
	mContext(c),
	mStack(&s)
{}

void GameState::requestStackPush(GameState::ID stateID) {
	mStack->push_state(stateID);
}

void GameState::requestStackPop() {
	mStack->pop_state();
}

void GameState::requestStackClear() {
	mStack->clear_stack();
}

AppContext GameState::getContext() const
{
	return mContext;
}
