#pragma once

#include "Component.h"
#include "C_Damageable.h"

class C_Health : public Component, public C_Damageable
{
public:
	C_Health(Object* owner);

	void DoDamage(const sf::Vector2f& dir, int amount) override;

private:
	int currentHealth;
};

