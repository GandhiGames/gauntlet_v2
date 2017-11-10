#pragma once

#include <map>
#include <queue>

#include "DungeonGenerator.h"

class PathFinder
{
public:
	PathFinder();
	~PathFinder();

	const std::vector<sf::Vector2f>& GetPath(DungeonGenerator& level, const sf::Vector2f& from, const sf::Vector2f& to);
	const std::vector<DungeonTile*> GetPathWithObstacles(DungeonGenerator& level, DungeonTile& from, DungeonTile& to);

	bool IsCached(DungeonGenerator& level, const sf::Vector2f& from, const sf::Vector2f& to);

private:
	std::map<std::pair<DungeonTile*, DungeonTile*>, std::vector<sf::Vector2f>> m_cached;

	int m_max;
	int m_currentExecutions;
	const std::vector<sf::Vector2f> m_emptyPositions;
};

