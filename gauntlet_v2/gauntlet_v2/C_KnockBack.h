#pragma once

#include "Component.h"
#include "C_Damageable.h"
#include "C_Velocity.h"

#include <memory>

class C_KnockBack : public Component, public C_Damageable
{
public:
	C_KnockBack(Object* owner);

	void Awake() override;

	void DoDamage(const sf::Vector2f& dir, int damage) override;

private:
	std::shared_ptr<C_Velocity> m_velocity;
	float force;
};

