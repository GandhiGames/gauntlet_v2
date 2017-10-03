#include "C_Seperation.h"
#include "Object.h"

const float C_Seperation::MAG_OFFSET = .01f;

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

	auto entities = GetEntitiesInSight(50.f, FOLLOWER_TAG);

	for(auto& obj : entities) 
	{
		sf::Vector2f toAgent = (m_owner->m_transform->GetPosition() - obj->m_transform->GetPosition());
		force += Mathf::normalize(toAgent) / (Mathf::magnitude(toAgent) * MAG_OFFSET);
	}

	Debug::Log(force);
	return force;
}