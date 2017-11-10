#include "PathFinder.h"
#include "NodeList.h"

PathFinder::PathFinder() : m_max(-1), m_emptyPositions()
{
}


PathFinder::~PathFinder()
{
}

/*
Use path caching as noted earlier. This may not pay off if your map is dynamic and forces the cache to clear often.

Use groups. Treat a group of units as one, designate one as leader and find a path for it. 
The other units can just follow the leader's footsteps with some minor path finding to those 
footsteps (so units appear to avoid obstacles in the main path too). 
Pick units that are next to each other for this to work and the paths from the 
subordinates to the leaders should be really small.

Trade off accuracy for interactivity. 
Don't calculate all of the paths at once, spread them out over time. 
Use a priority queue for units needing paths, pop a few, calculate their paths. 
You could even give your path finding code a time limit too. 
I did this in Combined with grouping and path caching, this change in my implementation had the biggest gains for me.
*/
const std::vector<sf::Vector2f>& PathFinder::GetPath(DungeonGenerator& level, const sf::Vector2f& from, const sf::Vector2f& to)
{
	std::vector<sf::Vector2f> targetPositions;

	NodeList& nodes = level.GetNodes();
	DungeonTile* startNode = nodes.GetTile(from);
	DungeonTile* goalNode = nodes.GetTile(to);

	if (!startNode || !goalNode || startNode == goalNode)
	{
		return m_emptyPositions;
	}

	auto key = std::make_pair(startNode, goalNode);
	auto cached = m_cached.find(key);
	if (cached != m_cached.end())
	{
		return cached->second;
	}

	// TODO: not great implementation as time dependent. Limits executions per frame rather than being time-independent.
	/*
	execution count no longer reset, implement time dependance (reset after 1 sec)
	if (m_max >= 0)
	{
		if (++m_currentExecutions > m_max)
		{
			return targetPositions;
		}
	}
	*/

	std::vector<DungeonTile*> openList;
	std::vector<DungeonTile*> closedList;
	std::vector<DungeonTile*> pathList;
	std::vector<DungeonTile*>::iterator position;
	DungeonTile* currentNode;

	// Reset all nodes.
	nodes.ResetNodes();

	sf::Vector2i size = level.GetSize();
	// Pre-compute our H cost (estimated cost to goal) for each node.
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			int rowOffset, heightOffset;
			DungeonTile* node = nodes.GetTile(i, j);

			heightOffset = abs(node->y - goalNode->y);
			rowOffset = abs(node->x - goalNode->x);

			node->H = heightOffset + rowOffset;
		}
	}

	// Add the start node to the open list.
	openList.push_back(startNode);

	// While we have values to check in the open list.
	while (!openList.empty())
	{
		// Find the node in the open list with the lowest F value and mark it as current.
		int lowestF = INT_MAX;

		for (DungeonTile* tile : openList)
		{
			if (tile->F < lowestF)
			{
				lowestF = tile->F;
				currentNode = tile;
			}
		}

		// Remove the current node from the open list and add it to the closed list.
		position = std::find(openList.begin(), openList.end(), currentNode);
		if (position != openList.end())
			openList.erase(position);

		closedList.push_back(currentNode);

		// Find all valid adjacent nodes.
		std::vector<DungeonTile*> adjacentTiles;
		DungeonTile* node;

		//TODO: remove unnesecarry extra call to GetTile
		// Top.
		node = nodes.GetTile(currentNode->x, currentNode->y - 1);
		if ((node != nullptr) && (node->type == DungeonTileType::Floor))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x, currentNode->y - 1));
		}

		// Right.
		node = nodes.GetTile(currentNode->x + 1, currentNode->y);
		if ((node != nullptr) && (node->type == DungeonTileType::Floor))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x + 1, currentNode->y));
		}

		// Bottom.
		node = nodes.GetTile(currentNode->x, currentNode->y + 1);
		if ((node != nullptr) && (node->type == DungeonTileType::Floor))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x, currentNode->y + 1));
		}

		// Left.
		node = nodes.GetTile(currentNode->x - 1, currentNode->y);
		if ((node != nullptr) && (node->type == DungeonTileType::Floor))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x - 1, currentNode->y));
		}

		// For all adjacent nodes.
		for (DungeonTile* node : adjacentTiles)
		{
			// If the node is our goal node.
			if (node->x == goalNode->x && node->y == goalNode->y)
			{
				// Parent the goal node to current.
				node->parentNode = currentNode;

				// Store the current path.
				while (node->parentNode != nullptr)
				{
					pathList.push_back(node);
					node = node->parentNode;
				}

				// Empty the open list and break out of our for loop.
				openList.clear();
				break;
			}
			else
			{
				// If the node is not in the closed list.
				position = std::find(closedList.begin(), closedList.end(), node);
				if (position == closedList.end())
				{
					// If the node is not in the open list.
					position = std::find(openList.begin(), openList.end(), node);
					if (position == openList.end())
					{
						// Add the node to the open list.
						openList.push_back(node);

						// Set the parent of the node to the current node.
						node->parentNode = currentNode;

						// Calculate G (total movement cost so far) cost.
						node->G = currentNode->G + 10;

						// Calculate the F (total movement cost + heuristic) cost.
						node->F = node->G + node->H;
					}
					else
					{
						// Check if this path is quicker that the other.
						int tempG = currentNode->G + 10;

						// Check if tempG is faster than the other. I.e, whether it's faster to go A->C->B that A->C.
						if (tempG < node->G)
						{
							// Re-parent node to this one.
							node->parentNode = currentNode;
						}
					}
				}
			}
		}
	}

	// Store the node locations as the enemies target locations.
	for (DungeonTile* tile : pathList)
	{
		targetPositions.push_back(level.GetTilePosition(tile->x, tile->y));
	}

	// Reverse the target position as we read them from goal to origin and we need them the other way around.
	std::reverse(targetPositions.begin(), targetPositions.end());

	auto pair = m_cached.insert(std::make_pair(key, targetPositions));

	return	pair.first->second;
}

//TODO: merge with out getpath method, remove duplicate code
// No optomisations currently performed when using this method. Only use during loading.
const std::vector<DungeonTile*> PathFinder::GetPathWithObstacles(DungeonGenerator& level, DungeonTile& from, DungeonTile& to)
{
	NodeList& nodes = level.GetNodes();
	DungeonTile* startNode = &from;
	DungeonTile* goalNode = &to;

	if (!startNode || !goalNode || startNode == goalNode)
	{
		return std::vector<DungeonTile*>();
	}

	std::vector<DungeonTile*> openList;
	std::vector<DungeonTile*> closedList;
	std::vector<DungeonTile*> pathList;
	std::vector<DungeonTile*>::iterator position;
	DungeonTile* currentNode;

	// Reset all nodes.
	nodes.ResetNodes();

	sf::Vector2i size = level.GetSize();
	// Pre-compute our H cost (estimated cost to goal) for each node.
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			int rowOffset, heightOffset;
			DungeonTile* node = nodes.GetTile(i, j);

			heightOffset = abs(node->y - goalNode->y);
			rowOffset = abs(node->x - goalNode->x);

			node->H = heightOffset + rowOffset;
		}
	}

	// Add the start node to the open list.
	openList.push_back(startNode);

	// While we have values to check in the open list.
	while (!openList.empty())
	{
		// Find the node in the open list with the lowest F value and mark it as current.
		int lowestF = INT_MAX;

		for (DungeonTile* tile : openList)
		{
			if (tile->F < lowestF)
			{
				lowestF = tile->F;
				currentNode = tile;
			}
		}

		// Remove the current node from the open list and add it to the closed list.
		position = std::find(openList.begin(), openList.end(), currentNode);
		if (position != openList.end())
			openList.erase(position);

		closedList.push_back(currentNode);

		// Find all valid adjacent nodes.
		std::vector<DungeonTile*> adjacentTiles;
		DungeonTile* node;

		//TODO: remove unnesecarry extra call to GetTile
		// Top.
		node = nodes.GetTile(currentNode->x, currentNode->y - 1);
		if ((node != nullptr))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x, currentNode->y - 1));
		}

		// Right.
		node = nodes.GetTile(currentNode->x + 1, currentNode->y);
		if ((node != nullptr))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x + 1, currentNode->y));
		}

		// Bottom.
		node = nodes.GetTile(currentNode->x, currentNode->y + 1);
		if ((node != nullptr))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x, currentNode->y + 1));
		}

		// Left.
		node = nodes.GetTile(currentNode->x - 1, currentNode->y);
		if ((node != nullptr))
		{
			adjacentTiles.push_back(nodes.GetTile(currentNode->x - 1, currentNode->y));
		}

		// For all adjacent nodes.
		for (DungeonTile* node : adjacentTiles)
		{
			// If the node is our goal node.
			if (node->x == goalNode->x && node->y == goalNode->y)
			{
				// Parent the goal node to current.
				node->parentNode = currentNode;

				// Store the current path.
				while (node->parentNode != nullptr)
				{
					pathList.push_back(node);
					node = node->parentNode;
				}

				// Empty the open list and break out of our for loop.
				openList.clear();
				break;
			}
			else
			{
				// If the node is not in the closed list.
				position = std::find(closedList.begin(), closedList.end(), node);
				if (position == closedList.end())
				{
					// If the node is not in the open list.
					position = std::find(openList.begin(), openList.end(), node);
					if (position == openList.end())
					{
						// Add the node to the open list.
						openList.push_back(node);

						// Set the parent of the node to the current node.
						node->parentNode = currentNode;

						// Calculate G (total movement cost so far) cost.
						node->G = currentNode->G + 10;

						// Calculate the F (total movement cost + heuristic) cost.
						node->F = node->G + node->H;
					}
					else
					{
						// Check if this path is quicker that the other.
						int tempG = currentNode->G + 10;

						// Check if tempG is faster than the other. I.e, whether it's faster to go A->C->B that A->C.
						if (tempG < node->G)
						{
							// Re-parent node to this one.
							node->parentNode = currentNode;
						}
					}
				}
			}
		}
	}

	// Reverse the target position as we read them from goal to origin and we need them the other way around.
	//std::reverse(pathList.begin(), pathList.end());

	return pathList;
}

bool PathFinder::IsCached(DungeonGenerator& level, const sf::Vector2f& from, const sf::Vector2f& to)
{
	NodeList& nodes = level.GetNodes();

	DungeonTile* startNode = nodes.GetTile(from);
	DungeonTile* goalNode = nodes.GetTile(to);

	auto key = std::make_pair(startNode, goalNode);
	auto cached = m_cached.find(key);
	return (cached != m_cached.end());
}
