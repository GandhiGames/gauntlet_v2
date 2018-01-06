#include "DungeonGenerator.h"
#include "Debug.h"

DungeonGenerator::DungeonGenerator(SharedContext& context) :
	m_transitionSteps(2),
	m_connected(true),
	m_chanceToBecomeWall(0.4f),
	m_backgroundToWallConversion(4),
	m_wallToBackgroundConversion(3),
	m_nodes(DUNGEON_WIDTH, DUNGEON_HEIGHT),
	m_clusters(*this, *context.m_pathFinder)
{
	m_nodes.SetOffset(sf::Vector2f(0.f, 0.f));

	auto fileName = "../resources/tilesets/cavern_ruins_tileset.png";
	m_tilesetID = TextureManager::AddTexture(fileName);
	auto texture = TextureManager::GetTexture(m_tilesetID);

	// Load all tiles.
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallTopLeft, sf::IntRect(448, 96, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallTopMiddle, sf::IntRect(480, 96, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallTopRight, sf::IntRect(512, 96, 32, 32)));

	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallMiddleLeft, sf::IntRect(448, 128, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallMiddle, sf::IntRect(480, 128, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallMiddleRight, sf::IntRect(512, 128, 32, 32)));

	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallBottomLeft, sf::IntRect(448, 160, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallBottomMiddle, sf::IntRect(480, 160, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::WallBottomRight, sf::IntRect(512, 160, 32, 32)));

	m_spriteLookup.emplace(std::make_pair(DungeonTileType::FacadeLeft, sf::IntRect(384, 64, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::FacadeMiddle, sf::IntRect(416, 64, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::FacadeRight, sf::IntRect(448, 64, 32, 32)));

	m_spriteLookup.emplace(std::make_pair(DungeonTileType::Floor, sf::IntRect(0, 0, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::Entry, sf::IntRect(96, 160, 32, 32)));
	m_spriteLookup.emplace(std::make_pair(DungeonTileType::Exit, sf::IntRect(96, 160, 32, 32)));
}


DungeonGenerator::~DungeonGenerator()
{
}

void DungeonGenerator::BuildSprite(DungeonTile& t)
{
	t.sprite.setTexture(TextureManager::GetTexture(m_tilesetID));
	t.sprite.setTextureRect(m_spriteLookup[t.type]);
}

void DungeonGenerator::Generate()
{
	InitialiseEnvironment();

	for (int step = 0; step < m_transitionSteps; step++)
	{
		PerformTransistionStep();
	}

	if (m_connected)
	{
		m_clusters.Identify();
		m_clusters.Connect();
	}

	RemoveExtraneous();

	// Identify clusters so they can be connected using Path manager 
	m_clusters.Identify();

	DefineWallTypes();

	BuildFacade();

	//PlaceEntranceAndExit();

	DisplayEnvironment();

	//m_clusters.CalculateMainCluster();
}

void DungeonGenerator::Draw(sf::RenderWindow &window, float timeDelta)
{
	m_nodes.Draw(window, timeDelta);
}

//TODO: pre-store dungeon size
sf::Vector2i DungeonGenerator::GetSize() const
{
	return sf::Vector2i(DUNGEON_WIDTH, DUNGEON_HEIGHT);
}

bool DungeonGenerator::IsSolid(DungeonTile& tile)
{
	return tile.type != DungeonTileType::Floor;
}

bool DungeonGenerator::IsSolid(int i, int j)
{
	DungeonTile* node = m_nodes.GetTile(i, j);

	if (node)
	{
		return IsSolid(*node);
	}
		
	return false;
}

bool DungeonGenerator::IsSolid(const sf::Vector2f& pos)
{
	DungeonTile* node = m_nodes.GetTile(pos);

	if (node)
	{
		return IsSolid(*node);
	}

	return false;
}

bool DungeonGenerator::CausesCollision(const sf::Vector2f& newPosition)
{
	// Get the tiles that the four corners player overlapping with.
	DungeonTile* overlappingTiles[4];

	//TODO: calculate on a per sprite basis.
	const float characterSize = 14.f;

	sf::Vector2f pos = newPosition;
	pos.x -= characterSize;
	pos.y -= characterSize;

	// Top left.
	overlappingTiles[0] = m_nodes.GetTile(sf::Vector2f(pos.x - characterSize, pos.y - characterSize));

	// Top right.
	overlappingTiles[1] = m_nodes.GetTile(sf::Vector2f(pos.x + characterSize, pos.y - characterSize));

	// Bottom left.
	overlappingTiles[2] = m_nodes.GetTile(sf::Vector2f(pos.x - characterSize, pos.y + characterSize));

	// Bottom right.
	overlappingTiles[3] = m_nodes.GetTile(sf::Vector2f(pos.x + characterSize, pos.y + characterSize));

	// If any of the overlapping tiles are solid there was a collision.
	for (int i = 0; i < 4; i++)
	{
		//Debug::DrawRect(GetTilePosition(overlappingTiles[i]->x, overlappingTiles[i]->y),
		//	sf::Vector2f(DUNGEON_TILE_SIZE, DUNGEON_TILE_SIZE), sf::Color::White);

		if (IsSolid(*overlappingTiles[i]))
		{
			Debug::DrawRect(GetTilePosition(overlappingTiles[i]->x, overlappingTiles[i]->y),
				sf::Vector2f(DUNGEON_TILE_SIZE, DUNGEON_TILE_SIZE), sf::Color::Red);
			return true;
		}
	}

	// If we've not returned yet no collisions were found.
	return false;
}

NodeList& DungeonGenerator::GetNodes()
{
	return m_nodes;
}

sf::Vector2f DungeonGenerator::GetRandomFloorLocation()
{
	// Declare the variables we need.
	int y(0), x(0);

	// Loop until we select a floor tile.
	while (m_nodes.GetTile(x, y)->type != DungeonTileType::Floor)
	{
		// Generate a random index for the row and column.
		x = std::rand() % DUNGEON_WIDTH;
		y = std::rand() % DUNGEON_HEIGHT;
	}

	// Convert the tile position to absolute position.
	sf::Vector2f tileLocation(GetTilePosition(x, y));

	// Create a random offset.
	//tileLocation.x += std::rand() % 8 - 4;
	//tileLocation.y += std::rand() % 8 - 4;

	return tileLocation;
}

void DungeonGenerator::InitialiseEnvironment()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			DungeonTileType cellType;

			if (IsEdge(x, y))
			{
				cellType = DungeonTileType::Wall;
			}
			else
			{
				cellType = Mathf::RandomValue() < m_chanceToBecomeWall ? DungeonTileType::Wall : DungeonTileType::Floor;
			}

			DungeonTile* node = m_nodes.GetTile(x, y);
			node->type = cellType;
			node->x = x;
			node->y = y;
		}
	}
}

void DungeonGenerator::PerformTransistionStep()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			int neighbourCount = CountTileWallNeighbours(x, y);

			DungeonTile* cell = m_nodes.GetTile(x, y);

			if (cell->type == DungeonTileType::Wall)
			{
				cell->type = (neighbourCount < m_wallToBackgroundConversion) ? DungeonTileType::Floor : DungeonTileType::Wall;
			}
			else
			{
				cell->type = (neighbourCount > m_backgroundToWallConversion) ? DungeonTileType::Wall : DungeonTileType::Floor;
			}
		}
	}
}

void DungeonGenerator::RemoveExtraneous()
{
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < DUNGEON_WIDTH; x++)
		{
			for (int y = 0; y < DUNGEON_HEIGHT; y++)
			{
				DungeonTile* cell = m_nodes.GetTile(x, y);

				if (cell->type != DungeonTileType::Wall)
				{
					continue;
				}

				if (IsExtraneousCell(x, y))
				{
					cell->type = DungeonTileType::Floor;
				}
			}
		}
	}

	RemoveLoneCells();
}

void DungeonGenerator::RemoveLoneCells()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			DungeonTile* cell = m_nodes.GetTile(x, y);

			if (cell->type != DungeonTileType::Wall)
			{
				continue;
			}

			if (IsLoneCell(x, y))
			{
				cell->type = DungeonTileType::Floor;
			}
		}
	}
}

void DungeonGenerator::DefineWallTypes()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			if (m_nodes.GetTile(x, y)->type == DungeonTileType::Wall)
			{
				DefineWallType(x, y);
			}
		}
	}
}

void DungeonGenerator::BuildFacade()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			DungeonTile* node = m_nodes.GetTile(x, y);

			if (node->type == DungeonTileType::WallBottomLeft)
			{
				DungeonTile* newNode = m_nodes.GetTile(x, y + 1);

				if (newNode)
				{
					newNode->type = DungeonTileType::FacadeLeft;
				}
			}
			else if (node->type == DungeonTileType::WallBottomMiddle)
			{
				DungeonTile* newNode = m_nodes.GetTile(x, y + 1);

				if (newNode)
				{
					newNode->type = DungeonTileType::FacadeMiddle;
				}
			}
			else if (node->type == DungeonTileType::WallBottomRight)
			{
				DungeonTile* newNode = m_nodes.GetTile(x, y + 1);

				if (newNode)
				{
					newNode->type = DungeonTileType::FacadeRight;
				}
			}
		}
	}
}

void DungeonGenerator::PlaceEntranceAndExit()
{
	int startI, endI;
	startI = endI = -1;

	while (startI == -1)
	{
		int index = std::rand() % DUNGEON_WIDTH;

		if (m_nodes.GetTile(index, DUNGEON_HEIGHT - 2)->type == DungeonTileType::Floor)
		{
			startI = index;
		}
	}

	while (endI == -1)
	{
		int index = std::rand() % DUNGEON_HEIGHT;

		if (m_nodes.GetTile(index, 1)->type == DungeonTileType::Floor)
		{
			endI = index;
		}
	}

	m_nodes.GetTile(startI, DUNGEON_HEIGHT - 2)->type = DungeonTileType::Entry;
	m_nodes.GetTile(endI, 1)->type = DungeonTileType::Exit;
}

void DungeonGenerator::DisplayEnvironment()
{
	for (int x = 0; x < DUNGEON_WIDTH; x++)
	{
		for (int y = 0; y < DUNGEON_HEIGHT; y++)
		{
			DungeonTile* node = m_nodes.GetTile(x, y);

			BuildSprite(*node);

			node->sprite.setPosition(GetTilePosition(x, y));
		}
	}
}

bool DungeonGenerator::IsEdge(int x, int y)
{
	return (x == 0 || x == DUNGEON_WIDTH - 1 || y == 0 || y == DUNGEON_HEIGHT - 1);
}

int DungeonGenerator::CountTileWallNeighbours(int x, int y)
{
	int wallCount = 0;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}

			DungeonTile* tile = m_nodes.GetTile(x + i, y + j);

			if (!tile || tile->type != DungeonTileType::Floor)
			{
				wallCount++;
			}
		}
	}
	return wallCount;
}



bool DungeonGenerator::IsExtraneousCell(int x, int y)
{
	bool cellBelow = m_nodes.ContainsTypeAtCoord(x, y + 1, DungeonTileType::Wall);
	bool cellLeft = m_nodes.ContainsTypeAtCoord(x - 1, y, DungeonTileType::Wall);
	bool cellRight = m_nodes.ContainsTypeAtCoord(x + 1, y, DungeonTileType::Wall);
	bool cellAbove = m_nodes.ContainsTypeAtCoord(x, y - 1, DungeonTileType::Wall);

	bool floorBelow = m_nodes.ContainsTypeAtCoord(x, y + 1, DungeonTileType::Floor);
	bool floorLeft = m_nodes.ContainsTypeAtCoord(x - 1, y, DungeonTileType::Floor);
	bool floorRight = m_nodes.ContainsTypeAtCoord(x + 1, y, DungeonTileType::Floor);
	bool floorAbove = m_nodes.ContainsTypeAtCoord(x, y - 1, DungeonTileType::Floor);

	if (!cellLeft && !cellRight && !cellBelow && !cellAbove) { return true; }
	if (!cellLeft && !cellAbove && !cellRight) { return true; }
	if (!cellLeft && !cellAbove && !cellBelow) { return true; }
	if (!cellBelow && !cellAbove && !cellRight) { return true; }
	if (!cellBelow && !cellLeft && !cellRight) { return true; }
	if (floorLeft && floorRight && cellAbove && cellBelow) { return true; }
	if (floorAbove && floorBelow && cellRight && cellLeft) { return true; }

	return false;
}

bool DungeonGenerator::IsLoneCell(int x, int y)
{
	bool cellBelow = m_nodes.ContainsTypeAtCoord(x, y + 1, DungeonTileType::Wall);
	bool cellLeft = m_nodes.ContainsTypeAtCoord(x - 1, y, DungeonTileType::Wall);
	bool cellRight = m_nodes.ContainsTypeAtCoord(x + 1, y, DungeonTileType::Wall);
	bool cellAbove = m_nodes.ContainsTypeAtCoord(x, y - 1, DungeonTileType::Wall);

	if (!cellLeft && !cellRight && !cellBelow && !cellAbove) { return true; }

	return false;
}

void DungeonGenerator::DefineWallType(int x, int y)
{
	DungeonTile* node = m_nodes.GetTile(x, y);

	if (!node) { return; }

	bool floorBelow = m_nodes.ContainsTypeAtCoord(x, y + 1, DungeonTileType::Floor);
	bool floorLeft = m_nodes.ContainsTypeAtCoord(x - 1, y, DungeonTileType::Floor);
	bool floorRight = m_nodes.ContainsTypeAtCoord(x + 1, y, DungeonTileType::Floor);
	bool floorAbove = m_nodes.ContainsTypeAtCoord(x, y - 1, DungeonTileType::Floor);

	bool floorAboveLeft = m_nodes.ContainsTypeAtCoord(x - 1, y - 1, DungeonTileType::Floor);

	if (!floorAboveLeft && floorLeft && !floorRight && !floorAbove && floorBelow)
	{
		node->type = DungeonTileType::WallBottomLeft;
	}
	else if (!floorBelow && !floorLeft && floorAbove && floorRight)
	{
		node->type = DungeonTileType::WallTopRight;
	}
	else if (!floorBelow && !floorLeft && floorAbove && !floorRight)
	{
		node->type = DungeonTileType::WallTopMiddle;
	}
	else if (!floorBelow && floorLeft && floorAbove && !floorRight)
	{
		node->type = DungeonTileType::WallTopLeft;
	}
	else if (floorBelow && !floorLeft && !floorAbove && floorRight)
	{
		node->type = DungeonTileType::WallBottomRight;
	}
	else if (floorBelow && !floorLeft && !floorAbove && !floorRight)
	{
		node->type = DungeonTileType::WallBottomMiddle;
	}
	else if (floorBelow && floorLeft && !floorAbove && !floorRight)
	{
		node->type = DungeonTileType::WallBottomLeft;
	}
	else if (!floorBelow && !floorAbove && floorLeft && !floorRight)
	{
		node->type = DungeonTileType::WallMiddleLeft;
	}
	else if (!floorBelow && !floorAbove && !floorLeft && floorRight)
	{
		node->type = DungeonTileType::WallMiddleRight;
	}
	else
	{
		node->type = DungeonTileType::WallMiddle;
	}
}

sf::Vector2f DungeonGenerator::GetTilePosition(int x, int y)
{
	sf::Vector2f location;

	const sf::Vector2f& origin = m_nodes.GetOffset();

	location.x = static_cast<float>(origin.x + (x * DUNGEON_TILE_SIZE) + (DUNGEON_TILE_SIZE / 2.f));
	location.y = static_cast<float>(origin.y + (y * DUNGEON_TILE_SIZE) + (DUNGEON_TILE_SIZE / 2.f));

	return location;
}

