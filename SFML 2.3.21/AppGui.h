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

/**
	Abstract Node class for the menu Tree. Follows the composite pattern.
	@see Leaf, Composite
*/
class Node : public sf::Drawable , public sf::Transformable{
	public:
	
		typedef std::unique_ptr<Node> ptr;
	
		/** 
		Handle the given event, the window is passed in case of a Mouse event to map the pixel to world coords
		*/
		virtual void handleEvent(const sf::Event& e, sf::RenderWindow& window) {};
		virtual void update(sf::Time time) {};


		virtual sf::FloatRect getGlobalBounds() const = 0;

		/**
		Get the absolute world position by applying all the parent Nodes' transforms
		*/
		sf::Vector2f getWorldPosition() const {
			return getWorldTransform() * sf::Vector2f();
		}
		/**
		Get the absolute world transform by applying all the parent Nodes' transforms
		*/
		sf::Transform getWorldTransform() const{
			sf::Transform transform = sf::Transform::Identity;
			for (const Node* node = this; node != nullptr; node = node->mParent)
				transform = node->getTransform() * transform;
			return transform;
		}
	protected:
		//to allow calling setParent from a Node reference/pointer
		friend class Composite;

		/**
		Sets the parent node.
		To avoid unilateral relations (a parent node that doesn't store its child) it shouldn't be called it directly.
		Instead it should be called indirectly from Composite::addChild.
		*/
		void setParent(Node* mParent) { this->mParent = mParent; };
		
		
		Node* mParent;
};

/**
Node class representing an inner node (with children)
It contains a vector of owning references to other nodes,
calls to draw() update() and handleEvent() will trigger a call from each child.
*/
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
			child->setParent(this);
			mChildren.push_back(std::move(child));
		}
		void removeChild(Node& child) {
			auto found = std::find_if(mChildren.begin(), mChildren.end(), [&child](Node::ptr& c) {return &(*c) == &child; });
			assert(found != mChildren.end());
			found->get()->setParent(nullptr);
			mChildren.erase(found);
		}


		virtual sf::FloatRect getGlobalBounds()const override {
			sf::FloatRect rect(getPosition(),sf::Vector2f(0.f,0.f));
			if (!mChildren.empty()){
				std::vector<sf::FloatRect> rects;
				rects.reserve(mChildren.size());

				std::for_each(mChildren.begin(), mChildren.end(), 
					[&rects](const GUI::Node::ptr& ptr) {
						rects.push_back(ptr->getGlobalBounds()); 
					}
				);

				rect.top = std::min_element(rects.begin(), rects.end(), [](sf::FloatRect& l, sf::FloatRect& r) {return l.top < r.top;})->top;
				rect.left = std::min_element(rects.begin(), rects.end(), [](sf::FloatRect& l, sf::FloatRect& r) {return l.left < r.left; })->left;
				auto heightRect = std::max_element(rects.begin(), rects.end(), [](sf::FloatRect& l, sf::FloatRect& r) {return l.top + l.height < r.top + r.height; });
				rect.width = heightRect->left + heightRect->width - rect.left;
				auto heightWidth = std::max_element(rects.begin(), rects.end(), [](sf::FloatRect& l, sf::FloatRect& r) {return l.left + l.width < r.left + r.width; });
				rect.height = heightRect->top + heightRect->height - rect.top;
			}
			return rect;
		};

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
/**
Node class representing a leaf node (with no children)
*/
class Leaf : public Node {
	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

};


class Button : public Node {
	public:
		Button(const AppContext& context, std::string text) { 
			this->setScale(1.f,1.f);
			m_texture_normal = &context.resources->textures.get(Texture::BUTTON);
			m_texture_mouseover = &context.resources->textures.get(Texture::BUTTON_MOUSEOVER);
			m_sprite.setTexture(*m_texture_normal,true);
			m_sprite.setScale(2.f, 2.f);
			m_text.setFont(context.resources->fonts.get(Font::arial));
			m_text.setCharacterSize(20);
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
//								std::cout << "pressed " << m_string << '\n';
								mCallback();
							}
						}
					}
				}break;
				case sf::Event::MouseMoved: {
					//get the mouse position on the screen
					auto mouse_position = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);

					//get the local position
					auto local_position = pixelToLocalCoords(win, this->getWorldTransform(), mouse_position);

					auto button_bounds = this->m_sprite.getGlobalBounds();

					if (button_bounds.contains(local_position)) {
//						std::cout << "mousover " << m_string << '\n';
						m_sprite.setTexture(*m_texture_mouseover);
					}
					else { 
						m_sprite.setTexture(*m_texture_normal);
					}
				}
			}
		};

		virtual sf::FloatRect getGlobalBounds()const override {
			
			auto rect = m_sprite.getGlobalBounds();
			auto pos = getPosition();
			auto scale = getScale();
			
			rect.left += pos.x;
			rect.top += pos.y;
			
			rect.width *= scale.x;
			rect.height *= scale.y;
			return rect;
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


class TextBox :public Leaf {
public:
	TextBox(const AppContext& context,const std::string& content, size_t linewidth, size_t char_size = 20u) :
		mText(
			textWrap(content,linewidth),
			context.resources->fonts.get(Font::consola),
			char_size)
	{};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
		states.transform *= this->getTransform();
		target.draw(mText, states);
	};

	virtual sf::FloatRect getGlobalBounds()const override {
		auto rect = mText.getGlobalBounds();
		auto pos = getPosition();
		auto scale = getScale();

		rect.left += pos.x;
		rect.top += pos.y;

		rect.width *= scale.x;
		rect.height *= scale.y;

		return rect;
	};

private:	
	sf::Text mText;

};

}