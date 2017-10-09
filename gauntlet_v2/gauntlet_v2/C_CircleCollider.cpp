#include "C_CircleCollider.h"
#include "Object.h"

C_CircleCollider::C_CircleCollider(Object* owner) : Component(owner)
{
}


C_CircleCollider::~C_CircleCollider()
{
}

void C_CircleCollider::SetRadius(float radius)
{
	m_radius = radius;
}

float C_CircleCollider::GetRadius() const
{
	return m_radius;
}

//TODO: intersection with box
Manifold C_CircleCollider::Intersects(std::shared_ptr<C_Collider2D> other)
{
	std::shared_ptr<C_CircleCollider> c = std::dynamic_pointer_cast<C_CircleCollider>(other);

	Manifold m;

	if (c)
	{
		// Setup a couple pointers to each object
		Object* A = m_owner;
		Object* B = c->m_owner;

		// Vector from A to B
		sf::Vector2f n = B->m_transform->GetPosition() - A->m_transform->GetPosition();

		float r = GetRadius() + c->GetRadius();
		r *= r;

		if (Mathf::sqrMagnitude(n) > r)
		{
			return m; // No Collision
		}

		// Circles have collided, now compute manifold
		float d = Mathf::magnitude(n); // perform actual sqrt

		// If distance between circles is not zero
		if (d != 0)
		{
			// Distance is difference between radius and distance
			m.resolve = (n / d) * (r - d);
		}

		// Circles are on same position
		else
		{
			// Choose random (but consistent) values
			m.resolve = sf::Vector2f(1.f, 0.f) * GetRadius();
		}
	}

	return m;
}

void C_CircleCollider::ResolveOverlap(const Manifold& m)
{
	m_owner->m_transform->SetPosition(m_owner->m_transform->GetPosition() + m.resolve);
}