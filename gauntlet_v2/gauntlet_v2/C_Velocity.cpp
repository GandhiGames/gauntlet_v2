#include "C_Velocity.h"
#include "Object.h"
#include "SharedContext.h"
#include "DungeonGenerator.h"

C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity({ 0.f, 0.f })
{
}


C_Velocity::~C_Velocity()
{
}

void C_Velocity::Update(float timeDelta)
{
	m_owner->m_transform->AddPosition(m_velocity);
}

void C_Velocity::Set(sf::Vector2f& velocity)
{
	if (abs(velocity.x) > 0.f || abs(velocity.y) > 0.f)
	{
		DungeonGenerator* level = m_owner->m_context.m_level;
		auto pos = m_owner->m_transform->GetPosition();

		// Calculate horizontal movement.
		if (level->CausesCollision(sf::Vector2f(pos.x + velocity.x, pos.y)))
		{
			velocity.x = 0.f;
		}

		// Calculate horizontal movement
		if (level->CausesCollision(sf::Vector2f(pos.x, pos.y + velocity.y)))
		{
			velocity.y = 0.f;
		}

		m_velocity = velocity;
	}
	else
	{
		m_velocity = velocity;
	}

}

const sf::Vector2f& C_Velocity::Get() const
{
	return m_velocity;
}
