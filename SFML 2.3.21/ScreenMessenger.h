#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include "ScreenMessage.h"
class ScreenMessenger :public sf::Drawable {
	struct TimedMessage {
		ScreenMessage	message;
		//time to wait until the message is displayed lmAO
		sf::Time		start;
		//time of display
		sf::Time		duration;
	private:
		bool			started=false;
	public:
		inline bool isVisible()const {
			return start <= sf::Time::Zero;
		}
		inline bool ended()const {
			return duration <= sf::Time::Zero;
		}
		inline void update(sf::Time t) { 
			switch (started) {
				case false:
					started = ((start -= t) <= sf::Time::Zero);
					break;
				case true:
					duration -= t;
					break;
			} 
		};
	};

	std::vector<TimedMessage> mMessages;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

public:
	void	update(sf::Time t);

	void	pushMessage(ScreenMessage&& message, sf::Time time);
	void	pushMessage(const ScreenMessage& message, sf::Time time);

	void	clear();
};