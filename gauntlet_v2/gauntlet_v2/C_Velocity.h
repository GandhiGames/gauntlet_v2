#pragma once

#include "C_Transform.h"
#include "C_Updateable.h"

class C_Velocity : public Component, public C_Updateable
{
	friend class Object;

public:
	C_Velocity(Object* owner);
	~C_Velocity();

	void Update(float timeDelta) override;

	void Set(sf::Vector2f& velocity);
	const sf::Vector2f& Get() const;

private:
	sf::Vector2f m_velocity;
};


