#include "C_Health.h"
#include "Object.h"


C_Health::C_Health(Object* owner) : Component(owner), currentHealth(5)
{
}

void C_Health::DoDamage(const sf::Vector2f& dir, int amount)
{
	currentHealth -= amount;

	if (currentHealth < 0)
	{
		m_owner->Destroy();
	}
}