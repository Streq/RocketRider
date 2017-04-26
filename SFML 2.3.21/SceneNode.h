#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <assert.h>
class SceneNode : public sf::Drawable, public sf::Transformable{
	public:
	using Ptr = std::unique_ptr<SceneNode>;
	virtual void					draw(sf::RenderTarget& target, sf::RenderStates states) const override {};
	void							addChild(Ptr p) { mChildren.push_back(std::move(p)); } 
	Ptr								removeChild(const SceneNode& node) {
		auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
		assert(found != mChildren.end());

		Ptr result = std::move(*found);
		result->mParent = nullptr;
		mChildren.erase(found);
		return result;
	}
	private:
	std::vector<Ptr>				mChildren;
	SceneNode*						mParent;
};