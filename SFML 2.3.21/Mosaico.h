#include <SFML/Graphics.hpp>
#include "Conversions.h"
/*
This class is really just a wrapper around a Sprite to make it have "infinite" boundaries
*/
class Mosaico:public sf::Drawable,public sf::Transformable{
	public: 
		enum Flags{
			infiniteLeft  = 1<<0,			//Left is practically infinite
			infiniteUp	  = 1<<1, 			//Top is practically infinite
			infiniteRight = 1<<2,			//Width is practically infinite
			infiniteDown  = 1<<3,			//Height is practically infinite
		};
	private:
		mutable sf::VertexArray vertices;
		sf::VertexArray			original_vertices;
		unsigned				flags;
		const sf::Texture*		texture;
	public:
		//Initialize like a sprite
		explicit Mosaico(const sf::Texture& texture, unsigned flags = 0);
		Mosaico(const sf::Texture& texture, const sf::FloatRect& textRect, unsigned flags = 0);
		Mosaico();
		void			setTextureRect(const sf::FloatRect& rect);
		void			setTexture(const sf::Texture& texture, bool resetRect=false);
		void 			draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	
	private:
		static void setTextureRect(sf::VertexArray& verts, const sf::FloatRect& rect);
		static void setTextureRectOnly(sf::VertexArray& verts, const sf::FloatRect&);
		static void setDisplayRect(sf::VertexArray& verts, const sf::FloatRect& rect);
		//fills the specified rect space with the sprite
		void fillRect(const sf::FloatRect& rect) const;
};