#pragma once

#include "C_MovementBehavior.h"

class C_Seperation : public C_MovementBehavior
{
public:
	C_Seperation(Object* owner);
	~C_Seperation();

	const sf::Vector2f GetForce() override;

	static const float MAG_OFFSET;
};

