#pragma once

#include <memory>

#include "Animation.h"
#include "Util.h"

class AnimationGroup
{
public:
	void AddAnimation(MOVEMENT_DIRECTION dir, Animation& animation);
	void SetDirection(MOVEMENT_DIRECTION dir);
	void Draw(sf::RenderWindow &window, float timeDelta);
	void Update(const sf::Vector2f pos);
	void Reset();
	std::vector<std::shared_ptr<Animation>> GetAnimations(MOVEMENT_DIRECTION dir);
	std::vector<std::shared_ptr<Animation>> GetAnimations(SPRITE_TYPE animationType);

private:
	//TODO: get rid of the outdated m_ notation!
	std::map<MOVEMENT_DIRECTION, std::vector<std::shared_ptr<Animation>>> m_animations;
	std::vector<std::shared_ptr<Animation>>* m_curAnimation;
};

