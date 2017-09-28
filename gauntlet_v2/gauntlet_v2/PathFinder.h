#pragma once

#include <map>
#include <queue>

#include "Level.h"

class PathFinder
{
public:
	PathFinder(Level& level);
	~PathFinder();

	std::vector<sf::Vector2f> GetPath(const sf::Vector2f& from, const sf::Vector2f& to);

	void ResetExecutionCount();

	bool IsCached(const sf::Vector2f& from, const sf::Vector2f& to);

private:
	Level& m_level;
	std::map<std::pair<Tile*, Tile*>, std::vector<sf::Vector2f>> m_cached;

	int m_max;
	int m_currentExecutions;
};

