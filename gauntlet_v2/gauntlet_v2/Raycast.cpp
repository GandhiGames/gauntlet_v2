#include "Raycast.h"
#include "SharedContext.h"
#include "Level.h"


Raycast::Raycast()
{
}


Raycast::~Raycast()
{
}


//TODO: increment step to tile size?
std::vector<sf::Vector2f> Raycast::BresenhamLine(const sf::Vector2f& from, const sf::Vector2f& to)
{
	// Optimization: it would be preferable to calculate in
	// advance the size of "result" and to use a fixed-size array
	// instead of a list.
	std::vector<sf::Vector2f> result;

	int steps = 5 + 1;
	sf::Vector2f diff = to - from;
	float xStep = diff.x / steps;
	float yStep = diff.y / steps;

	if (xStep >= TILE_SIZE || yStep >= TILE_SIZE)
	{
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

// Raycast using Bresenham algorithm 
RaycastResult Raycast::Cast(SharedContext& context, const sf::Vector2f& from, const sf::Vector2f& to)
{
	RaycastResult result;
	result.collision = false;

	//result.DoCollide = false;

	// Exit the function now if the ray length is 0
	if (from == to) 
	{
		//context.m_level->IsSolid((int)position.x, (int)position.y);
		//result.DoCollide = IsPointTraversable((int)position.X, (int)position.Y);
		//result.Position = position;
		
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
	
			if (context.m_level->IsSolid(rayPoint)) 
			{
				result.collision = true;
				return result;
				//result.Position = Vector.FromPoint(rayPoint);
				//result.DoCollide = true;
				//break;
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
