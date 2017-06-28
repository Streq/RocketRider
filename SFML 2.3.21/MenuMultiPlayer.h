#pragma once
#include "Menu.h"
#include "Level.h"
/*
-Dos jugadores SPLIT SCREEN
	-Pantalla de selección de mapa(es un menu más)(ver imagen de diseño)
		-Se cargan los niveles en un array de niveles
		-Y la imagen aparte
		-Slide para elegir mapa
		-Slide para elegir cantidad de rounds
		-Botón jugar
		-Las selecciones se guardan en el contexto de la app para que Game pueda inicializar desde ahí	
	-Juego	
		-Recibe por parámetro el mapa a cargar y la cantidad de rounds
		-Dos vistas con dos viewport
			-Duplicar aspect ratio horizontal así entra todo pillín pillón.
		-Si un personaje muere respawnea en donde empieza (a diferencia del otro modo, que reiniciaba el nivel).	
		-El primero en llegar al final triggerea una pantallita diciendo quién ganó.
		-Tocar el spawn_point de un jugador cuando este muere triggerea la muerte del otro jugador
*/
class MenuMultiPlayer: public Menu {
public:
	MenuMultiPlayer(GameStack& s, AppContext c);
//	bool handle_event(const sf::Event& e) override;
//	void draw() const override;
private:
	std::vector<Level>			maps;
	std::vector<sf::Texture>	map_textures;
	sf::Sprite					current_map;
	
};