#pragma once
struct  GameSettings {
	enum Modo { single, race, size };

	Modo		modo;

	//only matters on race mode
	unsigned	players;
	//race mode info
	unsigned	mapa;
	unsigned	rounds;
};