#include "GameSingle.h"

GameSingle::GameSingle(GameStack & s, AppContext context)
	: Game{s,context}
	, m_level_amount{0}
	, m_level_completed{false}
	, m_level_index{0}
	, m_mira{true}
	, m_won{false}
{
	init();
}

void GameSingle::init()
{
	Game::init();
	load_levels("Assets/Config/config.xml");
	next_level();
}

void GameSingle::handle_win_lose()
{
	
	for (auto& player : mPlayers) {
		if (player->goalCompleted()) {
			next_level();
			return;
		}
		if (player->isDead())
			goto_level(m_level_index);
	}
	
}


void GameSingle::goto_level(unsigned level)
{
	clear();
	loadLevel(mLevels[m_level_index = level]);
}

void GameSingle::load_levels(const std::string& path)
{
	load_levels_from_xml(mLevels, path);
	m_level_amount = mLevels.size();
}

void GameSingle::next_level()
{
	if (++m_level_index < static_cast<int>(m_level_amount)) {
		goto_level(m_level_index);
		m_level_completed = false;
		//m_display_message = true;
		//m_message_display_time = SF::LEVEL_TITLE_TIME;
	}
	else {
		m_won = true;
	}
}
