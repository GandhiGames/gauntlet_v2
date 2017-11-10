#include "C_BoxCollider.h"
#include "Object.h"

C_BoxCollider::C_BoxCollider(Object* owner) : Component(owner),
m_origin(Origin::Abs_Centre), m_offset(sf::Vector2f(0.f, 0.f))
{

}


C_BoxCollider::~C_BoxCollider()
{
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

const sf::FloatRect& C_BoxCollider::GetCollidable()
{
	SetPosition();

	return m_AABB;
}

void C_BoxCollider::SetOrigin(const Origin& origin) { m_origin = origin; }

void C_BoxCollider::SetCollidable(const sf::FloatRect& rect)
{
	m_AABB = rect;

	SetPosition();
}

//TODO: intersection with circle
Manifold C_BoxCollider::Intersects(std::shared_ptr<C_Collider2D> other)
{
	std::shared_ptr<C_BoxCollider> c = std::dynamic_pointer_cast<C_BoxCollider>(other);

	Manifold m;
	
	if (c)
	{
		const sf::FloatRect& rect1 = GetCollidable();
		const sf::FloatRect& rect2 = c->GetCollidable();

		if (rect1.intersects(rect2))
		{
			m.colliding = true;

			float xDiff = (rect1.left + (rect1.width / 2)) - (rect2.left + (rect2.width / 2));
			float yDiff = (rect1.top + (rect1.height / 2)) - (rect2.top + (rect2.height / 2));
			
			if (std::abs(xDiff) > std::abs(yDiff)) 
			{
				if (xDiff > 0) 
				{
					m.resolve = sf::Vector2f((rect2.left + rect2.width) - rect1.left, 0.f);
				}
				else 
				{
					m.resolve = sf::Vector2f(-((rect1.left + rect1.width) - rect2.left), 0.f);
				}
			}
			else 
			{
				if (yDiff > 0) 
				{
					m.resolve = sf::Vector2f(0.f, (rect2.top + rect2.height) - rect1.top);
				}
				else 
				{
					m.resolve = sf::Vector2f(0.f, -((rect1.top + rect1.height) - rect2.top));
				}
			}
		}

	}

	return m;
}

//TODO: resolve overlap with circle
void C_BoxCollider::ResolveOverlap(const Manifold& m)
{
	auto vel = m_owner->GetComponent<C_Velocity>();
	if (vel)
	{
		vel->Set(vel->Get() + (m.resolve * .5f));
	}
	//m_owner->m_transform->SetPosition(m_owner->m_transform->GetPosition() + m.resolve);
}

