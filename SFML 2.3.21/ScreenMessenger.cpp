#include "ScreenMessenger.h"

void ScreenMessenger::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto& message : mMessages)
		if (message.isVisible())
			target.draw(message.message, states);
}

void ScreenMessenger::update(sf::Time t)
{
	//pass time
	for (auto& message : mMessages) {
		message.update(t);
	}
	//erase messages that are no longer showing
	mMessages.erase(
		std::remove_if(
			mMessages.begin(), mMessages.end(),
			[](const TimedMessage& m) {return m.ended(); }
		)
		, mMessages.end()
	);
}
void ScreenMessenger::pushMessage(ScreenMessage && message, sf::Time time)
{
	TimedMessage m;
	m.message = std::forward<ScreenMessage>(message);
	m.duration = time;
	mMessages.push_back(std::move(m));
}

void ScreenMessenger::pushMessage(const ScreenMessage & message, sf::Time time)
{
	TimedMessage m;
	m.message = message;
	m.duration = time;
	mMessages.push_back(std::move(m));
}

void ScreenMessenger::clear()
{
	mMessages.clear();
}
