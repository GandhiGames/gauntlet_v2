#include "C_DirectionalAnimation.h"

#include "C_DirectionalAnimation.h"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : Component(owner),
m_currentState(ANIMATION_STATE::COUNT)
{
}

void C_DirectionalAnimation::Awake()
{
	m_sprite = m_owner->GetComponent<C_AnimatedSprite>();
	m_movement = m_owner->GetComponent<C_Velocity>();
	m_direction = m_owner->GetComponent<C_Direction>();
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	ANIMATION_STATE animState = m_currentState;

	const sf::Vector2f& velocity = m_movement->Get();
	const MOVEMENT_DIRECTION moveDir = m_direction->Get();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		animState = ANIMATION_STATE::WALK;
	}
	else
	{
		animState = ANIMATION_STATE::IDLE;
	}


	// Set the sprite.
	if (m_currentState != animState)
	{
		m_sprite->SetCurrentAnimation(animState);

		m_currentState = animState;
	}
}

