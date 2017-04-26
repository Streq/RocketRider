#include "TileTexture.h"
#include "Conversions.h"
/*
void TileTexture::resize_to_fit_view(const sf::View & view, bool horizontally, bool vertically)
{
	//reset texture
	m_sprite.setTexture(*m_sprite.getTexture(), true);
	//get global bounds
	const auto bounds = m_sprite.getGlobalBounds();
	sf::Vector2f size(1.f,1.f);
	auto textureSize = m_sprite.getTexture()->getSize();

	if (horizontally) {//stretch to fit the screen twice
		float fact = std::ceil(view.getSize().x / bounds.width);
		size.x = fact * textureSize.x * 2;
	}

	if (vertically){//stretch to fit the screen twice
		float fact = std::ceil(view.getSize().y / bounds.height);
		size.y = fact * textureSize.y * 2;
	}
	m_sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	
}

void TileTexture::adjust_to_view(const sf::View & view)
{
	sf::FloatRect safeRect{m_sprite.getGlobalBounds()};
	safeRect.width *= 0.5f;
	safeRect.height *= 0.5f;
	safeRect.top += safeRect.height * 0.5f;
	safeRect.left += safeRect.width * 0.5f;

	auto viewSize = view.getSize();
	auto viewCenter = view.getCenter();
	while (viewCenter.x < safeRect.left) {
		safeRect.left -= safeRect.width;
		m_sprite.move(-safeRect.width, 0.f);
	}while (viewCenter.x > safeRect.left + safeRect.width) {
		safeRect.left += safeRect.width;
		m_sprite.move(+safeRect.width, 0.f);
	}while (viewCenter.y < safeRect.top) {
		safeRect.top -= safeRect.height;
		m_sprite.move(0.f, -safeRect.height);
	}while (viewCenter.y > safeRect.top + safeRect.height) {
		safeRect.top += safeRect.height;
		m_sprite.move(0.f, +safeRect.height);
	}
}

void TileTexture::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
*/

void TileTexture::adjust_to_view(const sf::View & view)
{
	auto viewSize = view.getSize();
	sf::Vector2f viewCenter = view.getCenter() ;
	auto viewStart = viewCenter - viewSize * 0.5f;
	auto viewEnd = viewCenter + viewSize * 0.5f;

	
	
	//if (startbound.x) viewStart.x = std::max(viewStart.x, start.x*getScale().x + getPosition().x);
	//if (startbound.y) viewStart.y = std::max(viewStart.y, start.y*getScale().y + getPosition().y);

	//if (endbound.x) viewEnd.x = std::min(viewEnd.x, end.x*getScale().x + getPosition().x);
	//if (endbound.y) viewEnd.y = std::min(viewEnd.y, end.y*getScale().y + getPosition().y);
	//viewSize = (viewEnd - viewStart);
	sf::Vector2f texCoordSize = { (viewEnd.x - viewStart.x) / getScale().x, (viewEnd.y - viewStart.y) / getScale().y };
	
	sf::Vector2f texCoordCenter = { (viewStart.x + viewSize.x*0.5f) / getScale().x, (viewStart.y + viewSize.y*0.5f) / getScale().y };


	auto texStart = texCoordCenter - texCoordSize * 0.5f ;
	auto texEnd = texCoordCenter + texCoordSize * 0.5f;

	m_vertices[0].position = viewStart;
	m_vertices[1].position = { viewEnd.x, viewStart.y };
	m_vertices[2].position = viewEnd;
	m_vertices[3].position = { viewStart.x, viewEnd.y };
	
	m_vertices[0].texCoords = texStart;
	m_vertices[1].texCoords = { texEnd.x, texStart.y };
	m_vertices[2].texCoords = texEnd;
	m_vertices[3].texCoords = { texStart.x, texEnd.y };



}

void TileTexture::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = m_texture;
	target.draw(&m_vertices[0], 4, sf::Quads, states);
}
