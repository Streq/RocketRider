#pragma once
#include "GameObject.h"
#include "Resources.h"
#include "Constants.h"
#include "Player.h"
#include <functional>
#include "GoalDefinition.h"
class Goal : public GameObject {
	public:
	using Callback = std::function<void(Player*)>;
	Goal(const Resources& res, Callback callback, const b2Vec2& size = b2Vec2(1.f, 1.f), const b2Vec2& position = b2Vec2(0.f, 0.f));
	Goal(const Resources& res, Callback callback, GoalDefinition*);
	
	
	virtual void		initBody(b2World& world) final;
	virtual ObjectType	getType()const final;

	virtual void Step() override final;

	virtual void BeginContact(b2Contact* contact, bool id) final;


	private:
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const final;
	sf::Sprite			mSprite;
	b2Vec2				mSize;
	Callback			mCallback;
	bool				m_activate;

};