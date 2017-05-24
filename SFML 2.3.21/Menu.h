#pragma once
#include "GameState.h"
#include "AppGui.h"
class Menu:public GameState{
		
	public:
		Menu(GameStack& s, AppContext c);
		virtual void				draw() const override;
		virtual bool				handle_event(const sf::Event& e) override;
		virtual bool				update(sf::Time tiempo) override;
		virtual void				addButton(std::string name, std::function<void()> callback);
		virtual void				addText(std::string text, unsigned lineWidth, unsigned char_size = 20u);
		GUI::Composite&				getElements();
		

	protected:
		GUI::Composite mElements;
		void addElement(GUI::Node::ptr ptr, float x_offset, float y_offset);
};