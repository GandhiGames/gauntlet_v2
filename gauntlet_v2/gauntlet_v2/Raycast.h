#pragma once

#include <SFML\Graphics.hpp>

#include "RaycastResult.h"
#include "Debug.h"

class Raycast
{
	friend class SharedContext;
public:
	Raycast();
	~Raycast();

	static RaycastResult Cast(SharedContext& context, const sf::Vector2f& from, const sf::Vector2f& to);

	static std::vector<sf::Vector2f> BresenhamLine(const sf::Vector2f& from, const sf::Vector2f& to);
};

