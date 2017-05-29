#include "HUD.h"

void HUD::init(Player & player, AppContext context)
{
	this->player = &player;
	auto& textures = context.resources->textures;
	
	this->icons[actions::gas].setTexture(textures.get(Texture::GAS_ICON), true);
	this->icons[actions::rope].setTexture(textures.get(Texture::ROPE_ICON), true);
	this->icons[actions::steer].setTexture(textures.get(Texture::STEER_ICON), true);
	this->blocked.setTexture(textures.get(Texture::BLOCKED_ICON), true);
	for (int i = 0; i < actions::size; i++) {
		auto& icon = icons[i];
		icon.setScale(2.f, 2.f);
		icon.setPosition(icon.getGlobalBounds().width*i, 0.f);
	}
	

}

void HUD::update(sf::Time time)
{
	available[actions::gas] = player->hasFuel();
	available[actions::steer] = player->hasSteer();
	available[actions::rope] = player->hasRope();
}

void HUD::setPlayer(Player & player)
{
	this->player = &player;
}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	for (int i = 0; i < actions::size; i++) {
		auto& icon = icons[i];
		target.draw(icon,states);
		if (!available[i]) {
			auto state = states;
			state.transform *= icon.getTransform();
			target.draw(blocked, state);
		}
	}
}
