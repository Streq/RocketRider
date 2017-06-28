#pragma once
#include "Game.h"
class GameSingle : public Game {
public:
	GameSingle(GameStack& s, AppContext context);
	void init();

private:
	virtual void handle_win_lose()override;
	void goto_level(unsigned level);
	void load_levels(const std::string& path);
	void next_level();

	//1 Player stuff
	std::vector<Level>					mLevels;
	int									m_level_index;
	unsigned							m_level_amount;
	bool								m_level_completed;
	bool								m_won;
	bool								m_mira;
};