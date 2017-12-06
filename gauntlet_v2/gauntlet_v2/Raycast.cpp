#include "Raycast.h"
#include "SharedContext.h"
#include "DungeonGenerator.h"
#include "Object.h"

SharedContext* Raycast::m_context;

void Raycast::Initialise(SharedContext* context)
{
	m_context = context;
}

//TODO: increment step to tile size?
std::vector<sf::Vector2f> Raycast::BresenhamLine(const sf::Vector2f& from, const sf::Vector2f& to)
{
	// TODO: it would be preferable to calculate in
	// advance the size of "result" and to use a fixed-size array
	// instead of a list.
	std::vector<sf::Vector2f> result;
	
	sf::Vector2f diff = to - from;
	int steps = 0;

	float absDiffX = abs(diff.x);
	float absDiffY = abs(diff.y);

	if (absDiffX > absDiffY)
	{
		steps = (absDiffX / DUNGEON_TILE_SIZE) + 1;
	}
	else
	{
		steps = (absDiffY / DUNGEON_TILE_SIZE) + 1;
	}

	float xStep = diff.x / steps;
	float yStep = diff.y / steps;

	if (xStep >= DUNGEON_TILE_SIZE || yStep >= DUNGEON_TILE_SIZE)
	{
		//TODO: ensure this is never called and remove
		Debug::LogWarning("Raycast::Raycast not granular enough");
	}

	float newX = from.x;
	float newY = from.y;

	for (int i = 0; i <= steps; i++)
	{
		result.push_back(sf::Vector2f(newX, newY));

		newX += xStep;
		newY += yStep;
	}

	return result;

	/*
	float x0 = from.x;
	float y0 = from.y;
	float x1 = to.x;
	float y1 = to.y;

	bool steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep) 
	{
		Mathf::swap(x0, y0);
		Mathf::swap(x1, y1);
	}

	if (x0 > x1) 
	{
		Mathf::swap(x0, x1);
		Mathf::swap(y0, y1);
	}

	float deltax = x1 - x0;
	float deltay = abs(y1 - y0);
	float error = 0;
	float ystep;
	float y = y0;
	float xstep = 1.f; 

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (float x = x0; x <= x1; x++) 
	{
		if (steep) 
		{
			result.push_back(sf::Vector2f(y, x));
		}
		else
		{
			result.push_back(sf::Vector2f(x, y));
		}

		error += deltay;
		if (2 * error >= deltax) 
		{
			y += ystep;
			error -= deltax;
		}
	}

	return result;
	*/
}


std::vector<std::shared_ptr<Object>> Raycast::CircleCast(const sf::Vector2f& pos, float radius, const std::string& tag)
{
	auto entities = Object::GetObjects();

	std::vector<std::shared_ptr<Object>> localObjects;

	const float to = radius * radius;

	for (auto& obj : entities)
	{
		if (obj->m_tag->Get() == tag)
		{
			float distance = Mathf::distanceSqr(pos, obj->m_transform->GetPosition());

			if (distance < to)
			{
				localObjects.emplace_back(obj);
			}
		}
	}

	return localObjects;
}

// Raycast using Bresenham algorithm 
RaycastResult Raycast::Cast(const sf::Vector2f& from, const sf::Vector2f& to)
{
	RaycastResult result;
	result.collision = false;

	//result.DoCollide = false;

	// Exit the function now if the ray length is 0
	if (from == to) 
	{
		return result;
	}
	
	// Get the list of points from the Bresenham algorithm
	std::vector<sf::Vector2f> rayLine = BresenhamLine(from, to);

	if (rayLine.size() > 0) 
	{
		int rayPointIndex = 0;

		if (rayLine[0] != from)
		{
			rayPointIndex = rayLine.size() - 1;
		}

		// Loop through all the points starting from "position"
		while (true) 
		{
			sf::Vector2f& rayPoint = rayLine[rayPointIndex];
	
			//auto tile = m_context->m_level->GetNodes().GetTile(rayPoint);

			//Debug::DrawRect(m_context->m_level->GetTilePosition(tile->x, tile->y), sf::Vector2f(DUNGEON_TILE_SIZE, DUNGEON_TILE_SIZE));

			if (m_context->m_level->IsSolid(rayPoint)) 
			{
				result.collision = true;
				return result;
			}
			
			if (rayLine[0] != from) 
			{
				rayPointIndex--;
				if (rayPointIndex < 0)
				{
					break;
				}
			}
			else 
			{
				rayPointIndex++;
				if (rayPointIndex >= rayLine.size())
				{
					break;
				}
			}
		}
	}

	return result;
}
