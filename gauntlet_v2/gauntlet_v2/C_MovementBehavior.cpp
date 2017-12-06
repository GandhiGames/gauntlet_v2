#include "C_MovementBehavior.h"
#include "Object.h"
#include "SharedContext.h"
#include "S_Game.h"

C_MovementBehavior::C_MovementBehavior(Object* owner) : Component(owner), m_weight(1)
{
}


C_MovementBehavior::~C_MovementBehavior()
{
}

std::vector<std::shared_ptr<Object>> C_MovementBehavior::GetEntitiesInSight(float sightRadius, const std::string& tag) const
{
	auto entities = Object::GetObjects();

	const float radius = sightRadius * sightRadius;
	const sf::Vector2f& pos = m_owner->m_transform->GetPosition();

	std::vector<std::shared_ptr<Object>> retVals;

	for(auto& obj : entities) 
	{
		if ((obj->m_instanceID->Get() != m_owner->m_instanceID->Get()) && (obj->m_tag->Get() == tag))
		{
			float to = Mathf::distanceSqr(obj->m_transform->GetPosition(), pos);

			if (to < radius) 
			{
				retVals.push_back(obj);
			}
		}
	}

	return retVals;
}

void C_MovementBehavior::SetWeight(int weight)
{
	m_weight = weight;
}

int C_MovementBehavior::GetWeight() const
{
	return m_weight;
}
