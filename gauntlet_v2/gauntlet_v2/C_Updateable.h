#pragma once

class C_Updateable
{
	friend class Object;
public:
	C_Updateable();
	virtual ~C_Updateable() = 0;

	virtual void Update(float deltaTime) {}

	virtual void LateUpdate(float deltaTime) {}
};


