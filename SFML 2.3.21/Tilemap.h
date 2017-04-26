#pragma once
#include <SFML/Graphics.hpp>
#include "Conversions.h"
class Tilemap: public sf::Drawable, public sf::Transformable {
	public:
	void load(sf::Texture& tileset, const sf::Vector2u& tile_size, const uint8* tiles, unsigned width, unsigned height);

	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const override ;


	sf::VertexArray m_vertices;
	sf::Texture*	m_tileset;
	sf::Vector2u	m_tile_size;
	sf::Vector2u	m_tiles;
};