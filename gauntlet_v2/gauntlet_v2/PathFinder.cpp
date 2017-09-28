#include "PathFinder.h"


PathFinder::PathFinder(Level& level) : m_level(level), m_max(-1)
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
//TODO: move to last visible position (similiar to left 4 dead ai)
std::vector<sf::Vector2f> PathFinder::GetPath(const sf::Vector2f& from, const sf::Vector2f& to)
{
	std::vector<sf::Vector2f> targetPositions;

	Tile* startNode = m_level.GetTile(from);
	Tile* goalNode = m_level.GetTile(to);

	if (startNode == goalNode)
	{
		return targetPositions;
	}

	auto key = std::make_pair(startNode, goalNode);
	auto cached = m_cached.find(key);
	if (cached != m_cached.end())
	{
		return cached->second;
	}

	// TODO: not great implementation as time dependent. Limits executions per frame rather than being time-independent.
	/*
	if (m_max >= 0)
	{
		if (++m_currentExecutions > m_max)
		{
			return targetPositions;
		}
	}
	*/

	std::vector<Tile*> openList;
	std::vector<Tile*> closedList;
	std::vector<Tile*> pathList;
	std::vector<Tile*>::iterator position;
	Tile* currentNode;

	// Reset all nodes.
	m_level.ResetNodes();

	// Pre-compute our H cost (estimated cost to goal) for each node.
	for (int i = 0; i < m_level.GetSize().x; i++)
	{
		for (int j = 0; j < m_level.GetSize().y; j++)
		{
			int rowOffset, heightOffset;
			Tile* node = m_level.GetTile(i, j);

			heightOffset = abs(node->rowIndex - goalNode->rowIndex);
			rowOffset = abs(node->columnIndex - goalNode->columnIndex);

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

		for (Tile* tile : openList)
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
		std::vector<Tile*> adjacentTiles;
		Tile* node;

		// Top.
		node = m_level.GetTile(currentNode->columnIndex, currentNode->rowIndex - 1);
		if ((node != nullptr) && (m_level.IsFloor(*node)))
		{
			adjacentTiles.push_back(m_level.GetTile(currentNode->columnIndex, currentNode->rowIndex - 1));
		}

		// Right.
		node = m_level.GetTile(currentNode->columnIndex + 1, currentNode->rowIndex);
		if ((node != nullptr) && (m_level.IsFloor(*node)))
		{
			adjacentTiles.push_back(m_level.GetTile(currentNode->columnIndex + 1, currentNode->rowIndex));
		}

		// Bottom.
		node = m_level.GetTile(currentNode->columnIndex, currentNode->rowIndex + 1);
		if ((node != nullptr) && (m_level.IsFloor(*node)))
		{
			adjacentTiles.push_back(m_level.GetTile(currentNode->columnIndex, currentNode->rowIndex + 1));
		}

		// Left.
		node = m_level.GetTile(currentNode->columnIndex - 1, currentNode->rowIndex);
		if ((node != nullptr) && (m_level.IsFloor(*node)))
		{
			adjacentTiles.push_back(m_level.GetTile(currentNode->columnIndex - 1, currentNode->rowIndex));
		}

		// For all adjacent nodes.
		for (Tile* node : adjacentTiles)
		{
			// If the node is our goal node.
			if (node == goalNode)
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
	for (Tile* tile : pathList)
	{
		targetPositions.push_back(m_level.GetActualTileLocation(tile->columnIndex, tile->rowIndex));
	}

	// Reverse the target position as we read them from goal to origin and we need them the other way around.
	std::reverse(targetPositions.begin(), targetPositions.end());

	m_cached.insert(std::make_pair(key, targetPositions));

	return targetPositions;
}

bool PathFinder::IsCached(const sf::Vector2f& from, const sf::Vector2f& to)
{
	Tile* startNode = m_level.GetTile(from);
	Tile* goalNode = m_level.GetTile(to);

	auto key = std::make_pair(startNode, goalNode);
	auto cached = m_cached.find(key);
	return (cached != m_cached.end());
}

void PathFinder::ResetExecutionCount()
{
	m_currentExecutions = 0;
}