#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "C_Velocity.h"
#include "Input.h"

//TODO: rename to controller? or make work only with keyboard
class C_KeyboardController : public Component, public C_Updateable
{
	friend class Object;
	friend class Level;

public:
	C_KeyboardController(Object* owner);
	~C_KeyboardController();

	void Update(float timeDelta) override;


private:
	std::shared_ptr<C_Velocity> m_movement;

	//TODO: should keyboard controller decide movement speed? nope.
	int m_speed;
};

