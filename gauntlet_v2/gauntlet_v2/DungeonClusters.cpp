#include "DungeonClusters.h"
#include "DungeonGenerator.h"
#include "PathFinder.h"

DungeonClusters::DungeonClusters(DungeonGenerator& level, PathFinder& pathfinder) : 
	m_level(level), m_pathFinder(pathfinder)
{
}


DungeonClusters::~DungeonClusters()
{
}

void DungeonClusters::Identify()
{
	m_clusters.clear();

	NodeList& grid = m_level.GetNodes();

	const int width = grid.GetWidth();
	const int height = grid.GetHeight();
	const int length = width * height;

	std::vector<DungeonTile> floodFillArray(length);

	// Create a copy of all nodes as the nodes are altered during the identification process.
	for (int i = 0; i < length; i++)
	{
		//if (m_grid.IsValidCoord(x, y)) 
		//{
		DungeonTile* cellToCopy = grid.GetTile(i);
		DungeonTile* copy = &floodFillArray.at(i);
		copy->type = cellToCopy->type;
		copy->x = cellToCopy->x;
		copy->y = cellToCopy->y;
		//}	
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (floodFillArray.at(Mathf::to1DIndex(x, y, width)).type == DungeonTileType::Floor)
			{
				m_clusters.push_back(std::vector<DungeonTile>());
				FloodFillCluster(floodFillArray, x, y, width, height);
			}
		}
	}
}

void DungeonClusters::Connect()
{
	int mainClusterIndex = CalculateMainCluster();

	std::vector<DungeonTile>& mainCluster = m_clusters.at(mainClusterIndex);

	for (int clusterIndex = 0; clusterIndex < m_clusters.size(); clusterIndex++) 
	{
		if (clusterIndex != mainClusterIndex) 
		{
			std::vector<DungeonTile>& origCluster = m_clusters.at(clusterIndex);

			DungeonTile& origCell = origCluster.at((int)((origCluster.size() - 1) * Mathf::RandomValue()));

			DungeonTile& destCell = mainCluster.at((int)((mainCluster.size() - 1) * Mathf::RandomValue()));

			std::vector<DungeonTile*> path = m_pathFinder.GetPathWithObstacles(m_level, origCell, destCell);

			if (path.size() == 0) 
			{
				Debug::LogError("DungeonClusters: no path found");
			}
			else 
			{
				ConstructPath(path, DungeonTileType::Floor);
			}
		}
	}
}

void DungeonClusters::FloodFillCluster(std::vector<DungeonTile>& cells, int x, int y, int gridWidth, int gridHeight)
{
	DungeonTile& node = cells.at(Mathf::to1DIndex(x, y, gridWidth));

	// Only floor types should be considered.
	if (node.type != DungeonTileType::Floor)
	{
		return;
	}

	// Alter node state so it is not added again.
	node.type = DungeonTileType::Max;

	m_clusters.at(m_clusters.size() - 1).push_back(node);

	if (x > 0) 
	{
		FloodFillCluster(cells, x - 1, y, gridWidth, gridHeight);
	}

	if (x < gridWidth - 1) 
	{
		FloodFillCluster(cells, x + 1, y, gridWidth, gridHeight);
	}

	if (y > 0) 
	{
		FloodFillCluster(cells, x, y - 1, gridWidth, gridHeight);
	}

	if (y < gridHeight - 1) 
	{
		FloodFillCluster(cells, x, y + 1, gridWidth, gridHeight);
	}
}

int DungeonClusters::CalculateMainCluster()
{
	int mainClusterIndex = -1;
	int maxClusterSize = 0;

	for (int i = 0; i < m_clusters.size(); i++) 
	{
		std::vector<DungeonTile>& cluster = m_clusters.at(i);

		int cellCount = cluster.size();

		if (cellCount > maxClusterSize) 
		{
			maxClusterSize = cellCount;
			mainClusterIndex = i;
		}
	}

	return mainClusterIndex;
}

void DungeonClusters::ConstructPath(std::vector<DungeonTile*>& path, DungeonTileType pathType)
{
	NodeList& grid = m_level.GetNodes();

	for(auto p : path)
	{
		auto node = grid.GetTile(p->x, p->y);
		node->type = pathType;
	}
}
