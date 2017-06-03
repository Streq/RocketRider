#pragma once
#include "TileTexture.h"
#include "Mosaico.h"
struct BackgroundLayer : public sf::Drawable, public sf::Transformable{
//	TileTexture mTexture;
	Mosaico mTexture;
	float speed_factor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
		sf::View auxview = target.getView();

		sf::View view = applyDepthToView(auxview);
		states.transform *= this->getTransform();

		target.setView(view);
		target.draw(mTexture,states);
		target.setView(auxview);
	};
	sf::View applyDepthToView(const sf::View& view)const {
		sf::View ret(view);
		ret.setCenter(view.getCenter()*speed_factor);
		
		return ret; 
	}

};