#include "MenuMultiPlayer.h"
#include "GameStack.h"
MenuMultiPlayer::MenuMultiPlayer(GameStack & s, AppContext c)
	:Menu(s,c)
{
	load_levels_from_xml(this->maps, "Assets/Config/configrace.xml");
	{
		auto titulo{ std::make_unique<GUI::TextBox>(c,"Multiplayero", 20u) };
		addElement(std::move(titulo),0.f,0.f,false);
	}
	{
		auto menosJugadores{ std::make_unique<GUI::Button>(c,"<-") };
		menosJugadores->setCallback([c]() {
			auto& ply = c.settings->players;
			ply = std::max(ply - 1, 1u);
		});
		addElement(std::move(menosJugadores), 0.f, 0.f, true);
	}
	{
		auto jugadores{ std::make_unique<GUI::TextBox>(c,"Cantidad jugadores", 15u) };
		addElement(std::move(jugadores), 0.f, 0.f, false);
	}
	{
		auto masJugadores{ std::make_unique<GUI::Button>(c,"->") };
		masJugadores->setCallback([c](){
			auto& ply = c.settings->players;
			ply = std::min(ply + 1, 4u);
		});
		addElement(std::move(masJugadores), 0.f, 0.f, false);
	}
	{
		auto menosRounds{ std::make_unique<GUI::Button>(c,"<-") };
		menosRounds->setCallback([c]() {
			auto& rounds = c.settings->rounds;
			rounds= std::max(rounds - 1, 1u);
		});
		addElement(std::move(menosRounds), 0.f, 0.f, true);
	}
	{
		auto rounds{ std::make_unique<GUI::TextBox>(c,"Rounds", 15u) };
		addElement(std::move(rounds), 0.f, 0.f, false);
	}
	{
		auto masRounds{ std::make_unique<GUI::Button>(c,"->") };
		masRounds->setCallback([c]() {
			auto& rounds = c.settings->rounds;
			rounds = std::min(rounds + 1, 4u);
		});
		addElement(std::move(masRounds), 0.f, 0.f, false);
	}
	{
		auto menos{ std::make_unique<GUI::Button>(c,"<-") };
		menos->setCallback([c]() {
			auto& mapa = c.settings->mapa;
			mapa = std::max(mapa - 1, 1u);
		});
		addElement(std::move(menos), 0.f, 0.f, true);
	}
	{
		auto mapa{ std::make_unique<GUI::TextBox>(c,"Mapa", 15u) };
		addElement(std::move(mapa), 0.f, 0.f, false);
	}
	{
		auto mas{ std::make_unique<GUI::Button>(c,"->") };
		mas->setCallback([c]() {
			auto& mapa = c.settings->mapa;
			mapa = std::min(mapa + 1, 4u);
		});
		addElement(std::move(mas), 0.f, 0.f, false);
	}
	{
		auto jugar{ std::make_unique<GUI::Button>(c,"Jugar") };
		jugar->setCallback([this]() {
			auto& menu = *this;
			menu.requestStackPop();
			menu.requestStackPush(GameState::ID::GAME_RACE);
		});
		addElement(std::move(jugar), 0.f, 0.f, true);
	}
	{
		auto salir{ std::make_unique<GUI::Button>(c,"Volver") };
		salir->setCallback([this]() {
			auto& menu = *this;
			menu.requestStackPop();
		});
		addElement(std::move(salir), 0.f, 0.f, true);
	}
}
