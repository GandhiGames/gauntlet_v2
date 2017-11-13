#pragma once

#include <map>

#include "Animation.h"
#include "Util.h"

class AnimationGroup
{
public:
	AnimationGroup();
	~AnimationGroup();

	void AddAnimation(MOVEMENT_DIRECTION dir, Animation animation);

private:
	std::map<MOVEMENT_DIRECTION, Animation> m_animations;
};

