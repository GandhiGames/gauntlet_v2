#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "RaycastResult.h"
#include "Debug.h"

class Raycast
{
	friend class SharedContext;
	friend class Object;
public:
	static void Initialise(SharedContext* context);
	//TODO: move all to physics2d class.
	static RaycastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to);
	static std::vector<std::shared_ptr<Object>> CircleCast(const sf::Vector2f& pos, float radius, const std::string& tag);
	static std::vector<sf::Vector2f> BresenhamLine(const sf::Vector2f& from, const sf::Vector2f& to);

private:
	static SharedContext* m_context;
};

