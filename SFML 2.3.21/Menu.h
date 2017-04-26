#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
namespace Menu {

class Node : public sf::Drawable , public sf::Transformable{
	public:
	
	typedef std::unique_ptr<Node> ptr;

	virtual void handleEvent(const sf::Event& e) {};

	sf::Vector2f getWorldPosition() {
		return getWorldTransform() * sf::Vector2f();
	}

	sf::Transform getWorldTransform() {
		sf::Transform transform= sf::Transform::Identity;
		for (const Node* node = this; node != nullptr; node = node->mParent)
			transform = node->getTransform() * transform;
		return transform;
	}


	private:
	Node* mParent;
};


class Composite : public Node {
	std::vector<Node::ptr> mChildren;
	
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			for (auto it = begin(mChildren); it != end(mChildren); ++it) {
				const auto& node = *it->get();

				states.transform *= this->getTransform();
				target.draw(node, states);
			}
		}
	
		virtual void handleEvent(const sf::Event& e) {
			for (auto it = begin(mChildren); it != end(mChildren); ++it) {
				auto& node = *it->get();
				
				node.handleEvent(e);
			}
		}

};

class Leaf : public Node {
	std::vector<Node::ptr> mChildren;
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

};


class Button : public Leaf {
	public:
		virtual void			handleEvent(const sf::Event& e) override {
			switch (e.type) {
				case sf::Event::MouseButtonPressed: {
					switch (e.mouseButton.button) {
						case sf::Mouse::Button::Left: {
							
							//get the mouse position on the screen
							auto mouse_position = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
							
							//transform the sprite bounds by the world transform of the button;
							auto button_bounds = getWorldTransform().transformRect(m_sprite.getGlobalBounds());

							if (button_bounds.contains(mouse_position)) {
								std::cout << "pressed "<< m_text << '/n';
								mCallback();
							}
						}
					}
				}
			}
		};

		void					setCallback(const std::function<void()>& callback) { mCallback = callback; }
	

	private:
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	
		std::string				m_text;
		bool					m_selected;
		bool					m_clicked;


		sf::Sprite				m_sprite;
		sf::Texture*			m_clicked;
		sf::Texture*			m_normal;
		sf::Texture*			m_mouseover;

		std::function<void()>	mCallback;

};


}