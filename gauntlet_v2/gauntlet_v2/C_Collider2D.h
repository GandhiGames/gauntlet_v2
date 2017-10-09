#pragma once

#include<memory>
#include <SFML\Graphics.hpp>

#include "Bitmask.h"

enum class CollisionLayer
{
	Default = 0,
	Player = 1,
	Followers = 2
};

struct Manifold
{
	bool colliding = false;
	sf::Vector2f resolve = sf::Vector2f(0.f, 0.f);
};

class C_Collider2D 
{
public:
	C_Collider2D();
	~C_Collider2D();

	virtual Manifold Intersects(std::shared_ptr<C_Collider2D> other) = 0;
	virtual void ResolveOverlap(const Manifold& m) = 0;

	bool IsTrigger() const;
	void SetTrigger(bool isTrigger);

	CollisionLayer GetLayer() const;
	void SetLayer(CollisionLayer layer);

private:
	bool m_isTrigger;
	CollisionLayer m_layer;
};

