#pragma once
#include "TileTexture.h"

struct BackgroundLayer : public sf::Drawable{
	TileTexture mTexture;
	float speed_factor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
		sf::View auxview = target.getView();

		sf::View view = transformView(auxview);
		target.setView(view);
		target.draw(mTexture,states);
		target.setView(auxview);
	};
	sf::View transformView(const sf::View& view)const { return sf::View(view.getCenter()*speed_factor, view.getSize()); }

};