#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "Debug.h"
#include "Raycast.h"

class C_RaycastTest : public Component, public C_Updateable
{
	friend class Object;

public:
	C_RaycastTest(Object* owner);
	void Update(float deltaTime) override;
private:
	sf::Vector2f m_toOffset;
};
