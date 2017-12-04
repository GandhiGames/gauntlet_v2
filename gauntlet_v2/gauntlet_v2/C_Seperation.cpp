#include "C_Seperation.h"
#include "Object.h"

const float C_Seperation::MAG_OFFSET = .3f;

C_Seperation::C_Seperation(Object* owner) : C_MovementBehavior(owner)
{
	m_weight = 1;
}


C_Seperation::~C_Seperation()
{
}

const sf::Vector2f C_Seperation::GetForce()
{
	sf::Vector2f force(0.f, 0.f);

	const float sightRadius = 20.f * 20.f;

	for (auto obj : Object::GetObjects())
	{
		if (obj->m_tag->Get() == FOLLOWER_TAG)
		{
			if (obj->m_instanceID->Get() != m_owner->m_instanceID->Get())
			{
				sf::Vector2f toAgent = (m_owner->m_transform->GetPosition() - obj->m_transform->GetPosition());
				float to = Mathf::sqrMagnitude(toAgent);

				if (to < sightRadius)
				{
					force += Mathf::normalize(toAgent) / (sqrt(to) * MAG_OFFSET);
				}
			}
		}
	}

	return force;
}
