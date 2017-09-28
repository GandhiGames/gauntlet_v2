#pragma once

#include "Component.h"
#include "C_Updateable.h"

class C_Camera : public Component, public C_Updateable
{
public:
	C_Camera(Object* owner);
	~C_Camera();

	void LateUpdate(float deltaTime) override;

};

