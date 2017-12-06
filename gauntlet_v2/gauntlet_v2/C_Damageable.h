#pragma once

#include <SFML\Graphics.hpp>

class C_Damageable
{
public:
	virtual void DoDamage(const sf::Vector2f& dir, int damage) = 0;
};

