#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

/*struct TileTexture : public sf::Drawable, public sf::Transformable{
	sf::Sprite			m_sprite;
	bool				is_bounded_right, is_bounded_left, is_bounded_top, is_bounded_bot;
	sf::Vector2f		start;
	sf::Vector2f		end;


	void				resize_to_fit_view(const sf::View& view, bool horizontally=true, bool vertically=true);
	void				adjust_to_view(const sf::View& view);
	void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};*/

struct TileTexture : public sf::Drawable, public sf::Transformable {
	sf::Texture*		m_texture;
	
	sf::Vector2f		start;
	sf::Vector2<bool>	startbound;
	
	sf::Vector2f		end;
	sf::Vector2<bool>	endbound;
	
	sf::VertexArray		m_vertices;

	TileTexture() :m_vertices(sf::Quads, 4) {};

	void				adjust_to_view(const sf::View& view);
	void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};