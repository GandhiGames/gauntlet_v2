#pragma once

#include <SFML\Graphics.hpp>

#include "Bitmask.h"

enum class CollisionLayer
{
	Default = 0,
	Player = 1,
	Followers = 2
};

class C_Collider2D 
{
public:
	C_Collider2D();
	virtual ~C_Collider2D() = 0;

	virtual const sf::FloatRect& GetCollidable() const = 0;

	bool IsTrigger() const;
	void SetTrigger(bool isTrigger);

	CollisionLayer GetLayer() const;
	void SetLayer(CollisionLayer layer);

private:
	bool m_isTrigger;
	CollisionLayer m_layer;
};

