#pragma once

class Component
{
	friend class Object;
public:
	Component(Object* owner) : m_owner(owner)
	{
	}

	virtual ~Component() {}

public:
	Object* m_owner;

};
