#include "C_Transform.h"
#include "Object.h"

C_Transform::C_Transform(Object* owner) : Component(owner),
m_position({ 0.f, 0.f }) {}

C_Transform::~C_Transform() {}

// Sets the position of the object.
void C_Transform::SetPosition(sf::Vector2f position)
{
	m_position = position;
}

void C_Transform::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void C_Transform::AddPosition(sf::Vector2f position)
{
	m_position += position;
}

void C_Transform::SetX(float x)
{
	m_position.x = x;
}

void C_Transform::SetY(float y)
{
	m_position.y = y;
}

//TODO: can we use a reference?
const sf::Vector2f C_Transform::GetPosition() const
{
	if (m_parent == nullptr)
	{
		return m_position;
	}
	else
	{
		return m_parent->GetPosition() + m_position;
	}
}

void C_Transform::SetParent(std::shared_ptr<C_Transform> parent)
{
	m_parent = parent;

	m_parent->AddChild(m_owner->m_transform);
}

const std::shared_ptr<C_Transform> C_Transform::GetParent() const
{
	return m_parent;
}

void C_Transform::AddChild(std::shared_ptr<C_Transform> child)
{
	m_children.push_back(child);
}

void C_Transform::RemoveChild(std::shared_ptr<C_Transform> child)
{
	auto objIterator = m_children.begin();
	while (objIterator != m_children.end())
	{
		auto obj = **objIterator;

		if (obj.m_owner->m_instanceID->Get() == child->m_owner->m_instanceID->Get())
		{
			objIterator = m_children.erase(objIterator);
			break;
		}
		else
		{
			++objIterator;
		}
	}
}

const std::vector<std::shared_ptr<C_Transform>>& C_Transform::GetChildren() const
{
	return m_children;
}

