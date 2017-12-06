#include "AnimationGroup.h"


void AnimationGroup::AddAnimation(MOVEMENT_DIRECTION dir, Animation& animation)
{
	auto animationGroup = m_animations.find(dir);

	if (animationGroup == m_animations.end())
	{
		m_animations.insert(std::make_pair(dir, std::vector<std::shared_ptr<Animation>> { std::make_shared<Animation>(animation) }));
	}
	else
	{
		animationGroup->second.emplace_back(std::make_shared<Animation>(animation));
	}
}

void AnimationGroup::SetDirection(MOVEMENT_DIRECTION dir)
{
	auto animationGroup = m_animations.find(dir);

	if (animationGroup != m_animations.end())
	{
		m_curAnimation = &animationGroup->second;
	}
}

void AnimationGroup::Draw(sf::RenderWindow &window, float timeDelta)
{
	if (m_curAnimation)
	{
		for (auto& a : *m_curAnimation)
		{
			a->Draw(window, timeDelta);
		}
	}
}

void AnimationGroup::Update(const sf::Vector2f pos)
{
	if (m_curAnimation)
	{
		for (auto& a : *m_curAnimation)
		{
			a->SetPosition(pos);
		}
	}
}

void AnimationGroup::Reset()
{
	if (m_curAnimation)
	{
		for (auto& a : *m_curAnimation)
		{
			a->Reset();
		}
	}
}

std::vector<std::shared_ptr<Animation>> AnimationGroup::GetAnimations(MOVEMENT_DIRECTION dir)
{
	auto animationGroup = m_animations.find(dir);

	if (animationGroup != m_animations.end())
	{
		return animationGroup->second;
	}

	return std::vector<std::shared_ptr<Animation>>();
}

std::vector<std::shared_ptr<Animation>> AnimationGroup::GetAnimations(SPRITE_TYPE animationType)
{
	std::vector<std::shared_ptr<Animation>> animations;

	for (auto& animationsGroup : m_animations)
	{
		for (auto& animation : animationsGroup.second)
		{
			if (animation->GetType() == animationType)
			{
				animations.push_back(animation);
			}
		}
	}

	return animations;
}
