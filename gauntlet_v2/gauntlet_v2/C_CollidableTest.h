#pragma once

#include <cstdio>

#include "Component.h"
#include "C_Collidable.h"
#include "Debug.h"

class C_CollidableTest : public Component, public C_Collidable
{
public:
	C_CollidableTest(Object* owner) : Component(owner) {}

	void OnCollisionEnter(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n collision enter"); };
	void OnCollisionStay(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n collision stay"); };
	void OnCollisionExit(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n collision exit"); };


	void OnTriggerEnter(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n trigger enter"); };
	void OnTriggerStay(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n trigger stay"); };
	void OnTriggerExit(Object& owner, std::shared_ptr<Object> other) override { Debug::Log("\n trigger exit"); };
};
