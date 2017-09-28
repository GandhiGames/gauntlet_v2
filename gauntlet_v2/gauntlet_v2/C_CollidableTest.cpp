#include "C_CollidableTest.h"
#include "Object.h"

void C_CollidableTest::OnTriggerEnter(Object* owner, Object* other)
{
	other->Destroy();
}