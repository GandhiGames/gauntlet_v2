#include "C_BehaviorApplier.h"
#include "Object.h"

C_BehaviorApplier::C_BehaviorApplier(Object* owner) : Component(owner)
{
}

void C_BehaviorApplier::Awake()
{
	m_behaviors = m_owner->GetComponents<C_MovementBehavior>();
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_BehaviorApplier::Update(float deltaTime)
{
	sf::Vector2f force(0.f, 0.f);

	for (auto& b : m_behaviors)
	{
		force += b->GetForce() * (float)b->GetWeight();
	}

	if (Mathf::sqrMagnitude(force) > 0)
	{
		m_movement->Set(m_movement->Get() + force);
	}
}
