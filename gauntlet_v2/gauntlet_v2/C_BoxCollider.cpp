#include "C_BoxCollider.h"
#include "Object.h"

C_BoxCollider::C_BoxCollider(Object* owner) : Component(owner), 
m_origin(Origin::Abs_Centre), m_offset(sf::Vector2f(0.f, 0.f)) 
{

}


C_BoxCollider::~C_BoxCollider()
{
}

void C_BoxCollider::Update(float deltaTime)
{
	SetPosition();
}

void C_BoxCollider::SetPosition()
{
	const sf::Vector2f& pos = m_owner->m_transform->GetPosition();

	switch (m_origin)
	{
	case(Origin::Top_Left) :
	{
		m_AABB.left = pos.x + m_offset.x;
		m_AABB.top = pos.y + m_offset.y;
		break;
	}
	case(Origin::Abs_Centre) :
	{
		m_AABB.left = pos.x - (m_AABB.width / 2) + m_offset.x;
		m_AABB.top = pos.y - (m_AABB.height / 2) + m_offset.y;
		break;
	}
	case(Origin::Mid_Bottom) :
	{
		m_AABB.left = pos.x - (m_AABB.width / 2) + m_offset.x;
		m_AABB.top = pos.y - m_AABB.height + m_offset.y;
		break;
	}
	}
}


const sf::FloatRect& C_BoxCollider::GetCollidable() const
{
	return m_AABB;
}

void C_BoxCollider::SetOrigin(const Origin& origin) { m_origin = origin; }

void C_BoxCollider::SetCollidable(const sf::FloatRect& rect) 
{ 
	m_AABB = rect; 

	SetPosition();
}

