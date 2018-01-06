#include "NodeList.h"
#include "DungeonGenerator.h"

NodeList::NodeList(int width, int height) :
	m_offset(sf::Vector2f(0.f, 0.f)), m_width(width), m_height(height),
	m_grid(width * height)
{
}


NodeList::~NodeList()
{
}

void NodeList::Draw(sf::RenderWindow &window, float timeDelta)
{
	int length = m_width * m_height;
	for (int i = 0; i < length; i++)
	{
		//TODO: profile culling over drawing all.
		//if (!window.getDefaultView().getViewport().intersects(m_grid[i].sprite.getGlobalBounds()))
		//{
			window.draw(m_grid.at(i).sprite);
		//}
	}
}


void NodeList::SetOffset(sf::Vector2f offset)
{
	m_offset = offset;
}

const sf::Vector2f& NodeList::GetOffset()
{
	return m_offset;
}

DungeonTile* NodeList::GetTile(const sf::Vector2i& coord)
{
	if (!IsValidCoord(coord.x, coord.y))
	{
		return nullptr;
	}

	return &m_grid.at(Mathf::to1DIndex(coord.x, coord.y, m_width));
}

DungeonTile* NodeList::GetTile(const sf::Vector2f& position)
{
	/*
	// Convert the position to relative to the level grid.
	pos.x -= m_origin.x;
	pos.y -= m_origin.y;

	// Convert to a tile position.
	int tileColumn, tileRow;

	tileColumn = static_cast<int>(pos.x) / DUNGEON_TILE_SIZE;
	tileRow = static_cast<int>(pos.y) / DUNGEON_TILE_SIZE;
	*/
	// Convert to a tile position.
	//int tileColumn = (static_cast<int>(position.x - m_offset.x)) / DUNGEON_TILE_SIZE;
	//int tileRow = (static_cast<int>(position.y - m_offset.y)) / DUNGEON_TILE_SIZE;

	int tx = floor((position.x - m_offset.x) / DUNGEON_TILE_SIZE);
	int ty = floor((position.y - m_offset.y) / DUNGEON_TILE_SIZE);


	return GetTile(tx, ty);
}

DungeonTile* NodeList::GetTile(int x, int y)
{
	return GetTile(Mathf::to1DIndex(x, y, m_width));
}

DungeonTile* NodeList::GetTile(int onedCoord)
{
	if (onedCoord >= 0 && onedCoord < (m_width * m_height))
	{
		return &m_grid.at(onedCoord);
	}

	return nullptr;
}

void NodeList::ResetNodes()
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			DungeonTile& cell = m_grid.at(Mathf::to1DIndex(i, j, m_width));
			cell.parentNode = nullptr;
			cell.H = 0;
			cell.G = 0;
			cell.F = 0;
		}
	}
}

bool NodeList::IsValidCoord(int x, int y)
{
	bool validColumn, validRow;

	validColumn = ((x >= 0) && (x < m_width));
	validRow = ((y >= 0) && (y < m_height));

	return (validColumn && validRow);
}

bool NodeList::ContainsTypeAtCoord(int x, int y, DungeonTileType type)
{
	if (!IsValidCoord(x, y))
	{
		return false;
	}


	return m_grid.at(Mathf::to1DIndex(x, y, m_width)).type == type;
}


const int NodeList::GetWidth() const
{
	return m_width;
}

const int NodeList::GetHeight() const
{
	return m_height;
}

