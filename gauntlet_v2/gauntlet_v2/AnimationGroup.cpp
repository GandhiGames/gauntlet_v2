#include "AnimationGroup.h"



AnimationGroup::AnimationGroup()
{
}


AnimationGroup::~AnimationGroup()
{
}

void AnimationGroup::AddAnimation(MOVEMENT_DIRECTION dir, Animation animation)
{
	m_animations.emplace(std::make_pair(dir, animation));
}
