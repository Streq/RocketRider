#include "GameMultiplayer.h"

void GameMultiplayer::handle_win_lose()
{

}
void GameMultiplayer::init()
{
	Game::init();
	puntaje.resize(mSettings->players);
	load_map("Assets/Config/configrace.xml",mSettings->mapa);
	loadLevel(this->mLevel);
}
void GameMultiplayer::load_map(const std::string & path, int mapa)
{
	load_level_from_xml(this->mLevel, mapa, path);
}
GameMultiplayer::GameMultiplayer(GameStack & s, AppContext context)
	: Game{ s,context }
{
	init();
}
