#pragma once

enum class ANIMATION_STATE 
{
	WALK_UP,
	WALK_DOWN,
	WALK_RIGHT,
	WALK_LEFT,
	IDLE_UP,
	IDLE_DOWN,
	IDLE_RIGHT,
	IDLE_LEFT,
	COUNT
};

const std::string PLAYER_TAG = "Player";
const std::string FOLLOWER_TAG = "Follower";


//TODO: move to own header
class Mathf
{
public:
	/*
	* Math Helper Functions
	*/
	static sf::Vector2f normalize(sf::Vector2f v)
	{
		float l = magnitude(v);

		if (l != 0)
		{
			return v / l;
		}

		return v;
	}

	static sf::Vector2f normalize(sf::Vector2f v, float l)
	{
		if (l > 0)
		{
			return v / l;
		}

		return v;
	}

	static void swap(float& a, float& b)
	{
		float c = a;
		a = b;
		b = c;
	}

	static float sqrMagnitude(sf::Vector2f& v)
	{
		return v.x*v.x + v.y*v.y;
	}

	// Length
	static float magnitude(const sf::Vector2f& a)
	{
		return sqrt((a.x * a.x) + (a.y * a.y));
	}

	static float distance(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return (abs(sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)))));
	}

	static void truncate(sf::Vector2f& v, float max)
	{
		float sqrLength = sqrMagnitude(v);
		if (sqrLength > (max * max))
		{
			v = normalize(v);
			v *= max;
		}
	}

	// Dot product
	static float scalarProduct(const sf::Vector2f v1, const sf::Vector2f v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	static float invert(sf::Vector2f& v)
	{
		v.x = -v.x;
		v.y = -v.y;
	}

	sf::Vector2f componentProduct(const sf::Vector2f v1, const sf::Vector2f v2)
	{
		return sf::Vector2f(v1.x * v2.x, v1.y * v2.y);
	}
};

