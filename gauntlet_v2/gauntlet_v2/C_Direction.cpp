#include "C_Direction.h"
#include "Object.h"
#include "Debug.h"

C_Direction::C_Direction(Object* owner) : Component(owner), m_moveDir(MOVEMENT_DIRECTION::DOWN)
{
	//TODO: creates issue when components added out of order. Create Awake/Start methods and move these to them.
	m_movement = owner->GetComponent<C_Velocity>();
}

void C_Direction::Update(float timeDelta)
{
	const sf::Vector2f& velocity = m_movement->Get();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		if (abs(velocity.x) > abs(velocity.y))
		{
			if (velocity.x <= 0)
			{
				m_moveDir = MOVEMENT_DIRECTION::LEFT;
			}
			else
			{
				m_moveDir = MOVEMENT_DIRECTION::RIGHT;
			}
		}
		else
		{
			if (velocity.y <= 0)
			{
				m_moveDir = MOVEMENT_DIRECTION::UP;
			}
			else
			{
				m_moveDir = MOVEMENT_DIRECTION::DOWN;
			}
		}
	}
}

MOVEMENT_DIRECTION C_Direction::Get()
{
	return m_moveDir;
}