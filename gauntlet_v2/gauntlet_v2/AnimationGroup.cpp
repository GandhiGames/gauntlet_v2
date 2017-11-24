#include "AnimationGroup.h"



AnimationGroup::AnimationGroup() 
{
}


AnimationGroup::~AnimationGroup()
{
}

void AnimationGroup::AddAnimation(Animation& animation)
{
	m_animations.emplace_back(std::make_shared<Animation>(animation));
}


void AnimationGroup::Draw(sf::RenderWindow &window, float timeDelta)
{
	for (auto a : m_animations)
	{
		a->Draw(window, timeDelta);
	}
}

void AnimationGroup::Update(const sf::Vector2f pos)
{
	for (auto a : m_animations)
	{
		a->m_sprite.setPosition(pos);
	}
}

void AnimationGroup::Reset()
{
	for (auto a : m_animations)
	{
		a->Reset();
	}
}
