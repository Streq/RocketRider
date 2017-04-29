#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#include "Conversions.h"
#include "AppContext.h"
#include "text_utils.h"
namespace GUI {

class Node : public sf::Drawable , public sf::Transformable{
	public:
	
	typedef std::unique_ptr<Node> ptr;

	virtual void handleEvent(const sf::Event& e, sf::RenderWindow& window) {};

	sf::Vector2f getWorldPosition() const {
		return getWorldTransform() * sf::Vector2f();
	}

	sf::Transform getWorldTransform() const{
		sf::Transform transform = sf::Transform::Identity;
		for (const Node* node = this; node != nullptr; node = node->mParent)
			transform = node->getTransform() * transform;
		return transform;
	}

	void setParent(Node* mParent) { this->mParent = mParent; };
	protected:
	Node* mParent;
};


class Composite : public Node {
	std::vector<Node::ptr> mChildren;
	
	public:
		virtual void handleEvent(const sf::Event& e, sf::RenderWindow& win) override {
			for (auto it = begin(mChildren); it != end(mChildren); ++it) {
				auto& node = *it->get();

				node.handleEvent(e, win);
			}
		}
		void addChild(Node::ptr child) {
			mChildren.push_back(std::move(child));
		}
		void removeChild(Node& child) {
			auto found = std::find_if(mChildren.begin(), mChildren.end(), [&child](Node::ptr& c) {return &(*c) == &child; });
			
			assert(found != mChildren.end());
			
			found->get()->setParent(nullptr);
			mChildren.erase(found);
		}

	protected:
		virtual void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {};
	private:
		void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const{

			states.transform *= this->getTransform();
			for (auto it = begin(mChildren); it != end(mChildren); ++it) {
				const auto& node = *it->get();

				target.draw(node, states);
			}
		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final {
			drawSelf(target, states);
			drawChildren(target, states);
		}

};

class Leaf : public Node {
	std::vector<Node::ptr> mChildren;
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

};


class Button : public Leaf {
	public:
		Button(const AppContext& context, std::string text) { 
			
			m_texture_normal = &context.resources->textures.get(Texture::BUTTON);
			m_sprite.setTexture(*m_texture_normal,true);
			m_text.setFont(context.resources->fonts.get(Font::arial));
			m_text.setCharacterSize(10);
			m_text.setFillColor(sf::Color::White);
			
			
			setString(std::move(text));
		};
		virtual void handleEvent(const sf::Event& e, sf::RenderWindow& win) override {
			switch (e.type) {
				case sf::Event::MouseButtonPressed: {
					switch (e.mouseButton.button) {
						case sf::Mouse::Button::Left: {
							
							//get the mouse position on the screen
							auto mouse_position = sf::Vector2i(e.mouseButton.x, e.mouseButton.y);
							//get the local position
							auto local_position = pixelToLocalCoords(win, this->getWorldTransform(), mouse_position);

							auto button_bounds = this->m_sprite.getGlobalBounds();
							
							if (button_bounds.contains(local_position)) {
								std::cout << "pressed " << m_string << '\n';
								//mCallback();
							}
						}
					}
				}
			}
		};

		void					setCallback(const std::function<void()>& callback) { mCallback = callback; }
		void					setString(std::string str) { 
			m_string = std::move(str);
			m_text.setString(m_string);
			centerOrigin(m_text);
			m_text.setPosition(rectCenter(m_sprite.getGlobalBounds()));
		}

	private:
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			states.transform *= this->getWorldTransform();
			target.draw(m_sprite, states);
			target.draw(m_text, states);
		};
	
	
		std::string				m_string;
		bool					m_selected;
		bool					m_clicked;


		sf::Text				m_text;
		sf::Sprite				m_sprite;
		sf::Texture*			m_texture_clicked;
		sf::Texture*			m_texture_normal;
		sf::Texture*			m_texture_mouseover;
		
		std::function<void()>	mCallback;

};




}