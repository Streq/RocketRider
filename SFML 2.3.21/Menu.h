#pragma once
#include "GameState.h"
#include "AppGui.h"
class Menu:public GameState{
		
	public:
		Menu(GameStack& s, AppContext c);
		virtual void				draw() const override;
		virtual bool				handle_event(const sf::Event& e) override;
		virtual bool				update(sf::Time tiempo) override;
		GUI::Composite&				getElements();
		

	protected:
		GUI::Composite mElements;

};