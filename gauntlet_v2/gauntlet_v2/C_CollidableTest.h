#pragma once

#include <cstdio>

#include "Component.h"
#include "C_Collidable.h"

class C_CollidableTest : public Component, public C_Collidable
{
public:
	C_CollidableTest(Object* owner) : Component(owner) {}

	void OnCollisionEnter(Object* owner, Object* other) override { printf("\n collision enter"); };
	void OnCollisionStay(Object* owner, Object* other) override { printf("\n collision stay"); };
	void OnCollisionExit(Object* owner, Object* other) override { printf("\n collision exit"); };


	void OnTriggerEnter(Object* owner, Object* other) override; // { printf("\n trigger enter"); };
	void OnTriggerStay(Object* owner, Object* other) override { }// printf("\n trigger stay"); };
	void OnTriggerExit(Object* owner, Object* other) override { } //printf("\n trigger exit");
};
