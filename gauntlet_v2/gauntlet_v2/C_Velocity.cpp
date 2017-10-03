#include "C_Velocity.h"
#include "Object.h"

C_Velocity::C_Velocity(Object* owner) : Component(owner),
m_velocity({ 0.f, 0.f })
{
}


C_Velocity::~C_Velocity()
{
}

void C_Velocity::Update(float timeDelta)
{
	if (abs(m_velocity.x) > 0.f || abs(m_velocity.y) > 0.f)
	{
		m_owner->m_transform->SetPosition(
			m_owner->m_transform->GetPosition().x + m_velocity.x,
			m_owner->m_transform->GetPosition().y + m_velocity.y);
	}
}

void C_Velocity::Set(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

const sf::Vector2f& C_Velocity::Get() const
{
	return m_velocity;
}
