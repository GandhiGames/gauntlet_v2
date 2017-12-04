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

void C_DirectionalAnimation::Start()
{
	m_moveDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, ANIMATION_STATE::WALK_LEFT));
	m_moveDirections.insert(std::make_pair(MOVEMENT_DIRECTION::DOWN, ANIMATION_STATE::WALK_DOWN));
	m_moveDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, ANIMATION_STATE::WALK_RIGHT));
	m_moveDirections.insert(std::make_pair(MOVEMENT_DIRECTION::UP, ANIMATION_STATE::WALK_UP));

	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, ANIMATION_STATE::IDLE_LEFT));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::DOWN, ANIMATION_STATE::IDLE_DOWN));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, ANIMATION_STATE::IDLE_RIGHT));
	m_idleDirections.insert(std::make_pair(MOVEMENT_DIRECTION::UP, ANIMATION_STATE::IDLE_UP));
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	ANIMATION_STATE animState = m_currentState;

	const sf::Vector2f& velocity = m_movement->Get();
	const MOVEMENT_DIRECTION moveDir = m_direction->Get();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		animState = m_moveDirections[moveDir];
	}
	else
	{
		animState = m_idleDirections[moveDir];
	}


	// Set the sprite.
	if (m_currentState != animState)
	{
		m_sprite->SetCurrentAnimation(animState);

		m_currentState = animState;
	}
}

