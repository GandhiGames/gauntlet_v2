#pragma once

#include "C_Transform.h"
#include "C_Updateable.h"

class C_Movement : public Component, public C_Updateable
{
	friend class Object;

public:
	C_Movement(Object* owner);
	~C_Movement();

	void Update(float timeDelta) override;

	void SetVelocity(const sf::Vector2f& velocity);
	const sf::Vector2f& GetVelocity() const;

private:
	sf::Vector2f m_velocity;
};


