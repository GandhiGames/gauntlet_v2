#include "C_DirectionalAnimation.h"

#include "C_DirectionalAnimation.h"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : Component(owner),
m_currentState(ANIMATION_STATE::COUNT), m_prevDirection(MOVEMENT_DIRECTION::DOWN)
{
	m_sprite = owner->GetComponent<C_AnimatedSprite>();
	m_movement = owner->GetComponent<C_Velocity>();

	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, ANIMATION_STATE::IDLE_LEFT));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::DOWN, ANIMATION_STATE::IDLE_DOWN));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, ANIMATION_STATE::IDLE_RIGHT));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::UP, ANIMATION_STATE::IDLE_UP));
}


C_DirectionalAnimation::~C_DirectionalAnimation()
{
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	ANIMATION_STATE animState = m_currentState;

	const sf::Vector2f& velocity = m_movement->Get();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		if (abs(velocity.x) > abs(velocity.y))
		{
			if (velocity.x <= 0)
			{
				animState = ANIMATION_STATE::WALK_LEFT; 
				m_prevDirection = MOVEMENT_DIRECTION::LEFT;
			}
			else
			{
				animState = ANIMATION_STATE::WALK_RIGHT;
				m_prevDirection = MOVEMENT_DIRECTION::RIGHT;
			}
		}
		else
		{
			if (velocity.y <= 0)
			{
				animState = ANIMATION_STATE::WALK_UP;
				m_prevDirection = MOVEMENT_DIRECTION::UP;
			}
			else
			{
				animState = ANIMATION_STATE::WALK_DOWN;
				m_prevDirection = MOVEMENT_DIRECTION::DOWN;
			}
		}
	}
	else
	{
		animState = m_idleDirections[m_prevDirection];
	}


	// Set the sprite.
	if (m_currentState != animState)
	{
		m_sprite->SetCurrentAnimation(animState);

		m_currentState = animState;
	}
}

