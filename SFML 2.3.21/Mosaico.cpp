#include "Mosaico.h"

Mosaico::Mosaico(const sf::Texture & texture, unsigned flags)
	: original_vertices(sf::PrimitiveType::Quads,4)
	, vertices(sf::PrimitiveType::Quads, 4)
	, flags(flags) {
	setTexture(texture, true);
}

Mosaico::Mosaico(const sf::Texture& texture, const sf::FloatRect& textRect, unsigned flags) 
	: original_vertices(sf::PrimitiveType::Quads, 4)
	, vertices(sf::PrimitiveType::Quads, 4)
	, flags(flags)
	, texture(&texture)
{
	setTextureRect(textRect);
}

Mosaico::Mosaico()
	: original_vertices(sf::PrimitiveType::Quads, 4)
	, vertices(sf::PrimitiveType::Quads, 4)
{}



void Mosaico::setTexture(const sf::Texture& texture, bool resetRect) {
	this->texture = &texture;
	if (resetRect) {
		setTextureRect(sf::FloatRect(sf::Vector2f(), sf::Vector2f(texture.getSize())));
	}
}

void Mosaico::setTextureRect(const sf::FloatRect& textRect) {
	setTextureRect(original_vertices, textRect);
}


void Mosaico::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	//get the current view
	const auto& view = target.getView();
	//get transform
	sf::Transform transform(this->getInverseTransform());
	//apply inverse transform to view rect so it's in the same coord system as the Mosaico
	auto rect = transform.transformRect(rotatedViewRect(view));
	
	//fill the damn rect
	fillRect(rect);

	states.transform *= this->getTransform();
	states.texture = this->texture;
	target.draw(vertices, states);
}

void Mosaico::setTextureRect(sf::VertexArray & verts, const sf::FloatRect & textRect)
{
	const sf::Vector2f posi{ textRect.left,textRect.top };
	const sf::Vector2f posf{ textRect.left + textRect.width,textRect.top + textRect.height };
	verts[0].texCoords = posi;
	verts[0].position = posi;
	verts[1].texCoords = sf::Vector2f(posf.x, posi.y);
	verts[1].position = sf::Vector2f(posf.x, posi.y);
	verts[2].texCoords = posf;
	verts[2].position = posf;
	verts[3].texCoords = sf::Vector2f(posi.x, posf.y);
	verts[3].position = sf::Vector2f(posi.x, posf.y);
}

void Mosaico::setTextureRectOnly(sf::VertexArray & verts, const sf::FloatRect & textRect)
{
	const sf::Vector2f posi{ textRect.left,textRect.top };
	const sf::Vector2f posf{ textRect.left + textRect.width,textRect.top + textRect.height };
	verts[0].texCoords = posi;
	verts[1].texCoords = sf::Vector2f(posf.x, posi.y);
	verts[2].texCoords = posf;
	verts[3].texCoords = sf::Vector2f(posi.x, posf.y);
}

void Mosaico::setDisplayRect(sf::VertexArray & verts, const sf::FloatRect & rect)
{
	const sf::Vector2f posi{ rect.left,rect.top };
	const sf::Vector2f posf{ rect.left + rect.width,rect.top + rect.height };
	verts[0].position = posi;
	verts[1].position = sf::Vector2f(posf.x, posi.y);
	verts[2].position = posf;
	verts[3].position = sf::Vector2f(posi.x, posf.y);
}

void Mosaico::fillRect(const sf::FloatRect& rect) const {
	//calculate offset of topleft-most tile to be drawn from the topleft corner
	sf::Vector2f topleft{ rect.left, rect.top };
	sf::Vector2f botright{ rect.left + rect.width, rect.top + rect.height };

	sf::Vector2f tileSize{ original_vertices[2].position - original_vertices[0].position };
	sf::Vector2f offset{ positive_modulo(topleft.x,tileSize.x), positive_modulo(topleft.y,tileSize.y) };
	//we have the means, let's do this guyse
	//starts at the offset
	this->setTextureRectOnly(vertices, sf::FloatRect(offset,sf::Vector2f(rect.width,rect.height)));
	this->setDisplayRect(vertices, rect);
}