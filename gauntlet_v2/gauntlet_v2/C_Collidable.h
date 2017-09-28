#pragma once

class C_Collidable
{
	friend class Object;

public:
	C_Collidable();
	virtual ~C_Collidable() = 0;

	virtual void OnCollisionEnter(Object* owner, Object* other) {};
	virtual void OnCollisionStay(Object* owner, Object* other) {};
	virtual void OnCollisionExit(Object* owner, Object* other) {};

	virtual void OnTriggerEnter(Object* owner, Object* other) {};
	virtual void OnTriggerStay(Object* owner, Object* other) {};
	virtual void OnTriggerExit(Object* owner, Object* other) {};
};
