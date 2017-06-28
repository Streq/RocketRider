#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include "text_utils.h"
class ScreenMessage : public sf::Drawable, public sf::Transformable {
	public:
	void setString(const std::string& str) { m_string = str; update();};
	void setLineWidth(unsigned l) { m_line_width = l;  update();}
	void setText(const sf::Text& text) { m_text = text; update();};
	void setText(sf::Text&& text) { m_text = std::move(text);  update();}
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	
	private:
	void updateText();
	void updateBox();
	void update() { updateText(); updateBox(); };
	sf::Vector2f	m_size;
	std::string		m_string;
	sf::Text		m_text;
	bool			m_has_Rect;
	unsigned		m_line_width;
};