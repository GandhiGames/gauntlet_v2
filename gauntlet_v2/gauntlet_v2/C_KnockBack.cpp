#include "C_KnockBack.h"
#include "Object.h"


C_KnockBack::C_KnockBack(Object* owner) : Component(owner), force(350.f)
{
}

void C_KnockBack::Awake()
{
	m_velocity = m_owner->GetComponent<C_Velocity>();
}

void C_KnockBack::DoDamage(const sf::Vector2f& dir, int damage)
{
	m_velocity->AddForce(dir * force);
}
