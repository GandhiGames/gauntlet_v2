#pragma once

#include <vector>

#include "NodeList.h"

class DungeonClusters
{
	friend class DungeonGenerator;
	friend struct DungeonTile;
	friend class PathFinder;
public:
	DungeonClusters(DungeonGenerator& level, PathFinder& pathFinder);
	~DungeonClusters();

	void Identify();
	void Connect();
	int CalculateMainCluster();


private:
	void FloodFillCluster(std::vector<DungeonTile>& cells, int x, int y, int gridWidth, int gridHeight);

	void ConstructPath(std::vector<DungeonTile*>& path, DungeonTileType pathType);

private:
	DungeonGenerator& m_level;
	std::vector<std::vector<DungeonTile>> m_clusters;
	PathFinder& m_pathFinder;
};

