#pragma once

#include <memory>

#include "Animation.h"
#include "Util.h"

class AnimationGroup
{
public:
	AnimationGroup();
	~AnimationGroup();

	void AddAnimation(Animation& animation);

	void Draw(sf::RenderWindow &window, float timeDelta);
	void Update(const sf::Vector2f pos);
	void Reset();

private:
	//TODO: get rid of the outdated m_ notation!
	std::vector<std::shared_ptr<Animation>> m_animations;
};

