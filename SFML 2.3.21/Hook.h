#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
class Player;
class Hook: public sf::Drawable{

public:
						Hook(Player& mPlayer);
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states)const final;
	bool				isActive()const;
	void				activate(b2Body* target, b2Vec2 Local_Point);
	void				init();
	void				deactivate();
	
private:

	mutable sf::VertexArray mVec;
	Player&		 mPlayer;
	b2Body*		 mTarget;

	b2RopeJoint* mJointA;
	b2RopeJoint* mJointB;
	bool		 mActive;

	b2RopeJointDef mJointDef;
};