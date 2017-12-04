#pragma once

#include <vector>
#include <memory>

#include "Component.h"
#include "C_Updateable.h"
#include "C_MovementBehavior.h"
#include "C_Velocity.h"

class C_BehaviorApplier : public Component, public C_Updateable
{
public:
	C_BehaviorApplier(Object* owner);

	void Awake() override;
	void Update(float deltaTime) override;

private:
	std::vector<std::shared_ptr<C_MovementBehavior>> m_behaviors;
	std::shared_ptr<C_Velocity> m_movement;
};

