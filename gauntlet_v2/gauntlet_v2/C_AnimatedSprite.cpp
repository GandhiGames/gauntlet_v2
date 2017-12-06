#include "C_AnimatedSprite.h"
#include "Object.h"

C_AnimatedSprite::C_AnimatedSprite(Object* owner) : Component(owner),
m_animated(false), m_curDirection(MOVEMENT_DIRECTION::COUNT)
{
}

void C_AnimatedSprite::Awake()
{
	m_direction = m_owner->GetComponent<C_Direction>();
}

void C_AnimatedSprite::LateUpdate(float deltaTime)
{
	if (m_curAnimation)
	{
		MOVEMENT_DIRECTION curDir = m_direction->Get();

		if (curDir != m_curDirection)
		{
			m_curDirection = curDir;
			m_curAnimation->SetDirection(curDir);
			m_curAnimation->Reset();
		}

		m_curAnimation->Update(m_owner->m_transform->GetPosition());
	}
}

// Draws the object to the given render window.
void C_AnimatedSprite::Draw(sf::RenderWindow &window, float timeDelta)
{
	if (m_animated && m_curAnimation)
	{
		m_curAnimation->Draw(window, timeDelta);
	}
}

void C_AnimatedSprite::AddAnimation(ANIMATION_STATE state, AnimationGroup& animationGroup)
{
	auto inserted = m_animations.insert(std::make_pair(state, std::make_shared<AnimationGroup>(animationGroup)));

	if (!m_curAnimation)
	{
		m_curAnimation = inserted.first->second;

		m_animated = true;
	}
}

void C_AnimatedSprite::SetCurrentAnimation(ANIMATION_STATE state)
{
	auto animation = m_animations.find(state);
	if (animation != m_animations.end())
	{
		m_curAnimation = animation->second;
		m_curAnimation->SetDirection(m_curDirection);
		m_curAnimation->Reset();
	}
}

std::shared_ptr<AnimationGroup> C_AnimatedSprite::GetAnimation(ANIMATION_STATE state)
{
	auto animation = m_animations.find(state);
	if (animation != m_animations.end())
	{
		return animation->second;
	}

	return nullptr;
}

void C_AnimatedSprite::SetAnimated(bool animated)
{
	if (animated && !m_animated && m_curAnimation) // if we want to be animated but we are not currently animated.
	{
		m_curAnimation->Reset();
	}

	m_animated = animated;
}

bool C_AnimatedSprite::IsAnimated()
{
	return m_animated;
}

/*
const sf::Sprite* C_AnimatedSprite::GetSprite()
{
	if (m_curAnimation)
	{
		return &m_curAnimation->m_sprite;
	}

	return nullptr;
}
*/
