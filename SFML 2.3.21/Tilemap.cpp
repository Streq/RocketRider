#include "Tilemap.h"

void Tilemap::load(sf::Texture & tileset, const sf::Vector2u & tile_size, const uint8 * tiles, unsigned width, unsigned height)
{
	m_tileset = &tileset;
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(4 * width * height);
	m_tile_size = tile_size;
	m_tiles.x = width;
	m_tiles.y = height;
	auto htiles = m_tileset->getSize().x / m_tile_size.x;
	auto vtiles = m_tileset->getSize().y / m_tile_size.y;

	for (unsigned j = 0; j < height; j++) {
		for (unsigned i = 0; i < width; i++) {
			// get the current tile number
			int tileNumber = tiles[i + j * width];

			// find its position in the tileset texture
			int tu = tileNumber % htiles;
			int tv = tileNumber / (htiles);

			// if tile number out of range ignore
			if (tv >= vtiles){ tv = vtiles - 1; tu = htiles - 1; }

			// define world position
			auto* quads = &m_vertices[(i + j*width) * 4];
			quads[0].position = sf::Vector2f(i*m_tile_size.x, j*m_tile_size.y);
			quads[1].position = sf::Vector2f((i + 1)*m_tile_size.x, j*m_tile_size.y);
			quads[2].position = sf::Vector2f((i + 1)*m_tile_size.x, (j + 1)*m_tile_size.y);
			quads[3].position = sf::Vector2f(i*m_tile_size.x, (j + 1)*m_tile_size.y);

			// define texture textCoords
			quads[0].texCoords = sf::Vector2f(tu*m_tile_size.x, tv*m_tile_size.y);
			quads[1].texCoords = sf::Vector2f((tu + 1)*m_tile_size.x, tv*m_tile_size.y);
			quads[2].texCoords = sf::Vector2f((tu + 1)*m_tile_size.x, (tv + 1)*m_tile_size.y);
			quads[3].texCoords = sf::Vector2f(tu*m_tile_size.x, (tv + 1)*m_tile_size.y);



		}
	}
}

void Tilemap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//apply the transform
	states.transform *= this->getTransform();

	//apply the tileset texture
	states.texture = m_tileset;

	sf::Vector2f globalPos (getPosition().x - getOrigin().x * getScale().x, getPosition().y - getOrigin().y * getScale().y);

	//only draw what's inside the view
	auto view_rect = viewRect(target.getView());
	//view_rect.height *= 1.25f;
	//view_rect.width *= 1.25f;
	auto last_tile_pos = (m_tiles.y * m_tiles.x - 1) * 4;
	sf::FloatRect tilemap_rect (globalPos,m_vertices[last_tile_pos+2].position);
	//transform to global coords
	tilemap_rect.width *= getScale().x;
	tilemap_rect.height *= getScale().y;
	if (view_rect.intersects(tilemap_rect)) {
		//get the intersection rect
		auto inter = getIntersection(view_rect, tilemap_rect);
		//transform rect back to local coords
		inter.left -= tilemap_rect.left;
		inter.top -= tilemap_rect.top;
		inter.width /= getScale().x;
		inter.height /= getScale().y;

		//calculate tiles
		const sf::Vector2u start(std::floor(inter.left / m_tile_size.x), std::floor(inter.top / m_tile_size.y));
		const sf::Vector2u end(std::ceil((inter.width + inter.left) / m_tile_size.x), std::ceil((inter.height + inter.top) / m_tile_size.y));

		//draw said tiles
		for (size_t j = start.y; j < end.y; j++) {
			unsigned tilenum = (j * m_tiles.x + start.x) * 4;
			target.draw(&m_vertices[tilenum], (end.x-start.x) * 4, sf::PrimitiveType::Quads, states);
		}


		//draw the thang
		//target.draw((&m_vertices[0]), size, sf::PrimitiveType::Quads, states);
	}

}
