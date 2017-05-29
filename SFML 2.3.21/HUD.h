#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "AppContext.h"
#include <array>
class HUD :public sf::Drawable, public sf::Transformable {
public:
	enum actions{gas,steer,rope, size};

	void init(Player& player, AppContext context);
	void update(sf::Time time);
	void setPlayer(Player& player);
private:
	Player* player;
	std::array<sf::Sprite,size>			icons;
	std::array<bool,size>				available;
	
	sf::Sprite blocked;
	
private:
	void draw(sf::RenderTarget& target,sf::RenderStates states)const override;

};

