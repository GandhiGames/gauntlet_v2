#pragma once

#include "Component.h"
#include "C_Collider2D.h"

class C_CircleCollider : public Component, public C_Collider2D
{
public:
	C_CircleCollider(Object* owner);
	~C_CircleCollider();

	void SetRadius(float radius);
	float GetRadius() const;

	Manifold Intersects(std::shared_ptr<C_Collider2D> other) override;
	void ResolveOverlap(const Manifold& m) override;

private:
	float m_radius;
};

