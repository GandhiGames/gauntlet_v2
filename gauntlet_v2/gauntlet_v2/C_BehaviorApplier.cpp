#include "C_BehaviorApplier.h"
#include "Object.h"


C_BehaviorApplier::C_BehaviorApplier(Object* owner) : Component(owner)
{
	m_behaviors = m_owner->GetComponents<C_MovementBehavior>();
	m_movement = m_owner->GetComponent<C_Velocity>();
}


C_BehaviorApplier::~C_BehaviorApplier()
{
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
		sf::Vector2f acceleration = force / 1.f; // Mass

		Mathf::truncate(acceleration, 10.f); // m_maxSpeed;

		m_movement->Set(m_movement->Get() + (acceleration * deltaTime));
	}
}
