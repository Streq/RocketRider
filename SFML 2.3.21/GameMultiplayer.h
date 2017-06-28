#pragma once
#include "Game.h"
class GameMultiplayer : public Game {
	public:
		GameMultiplayer(GameStack & s, AppContext context);
		
	private:
		std::vector<int> puntaje;
		Level mLevel;
		
		virtual void handle_win_lose()override;
		virtual void init() override;
		void load_map(const std::string& path, int mapa);
};