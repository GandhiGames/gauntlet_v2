#pragma once

#include "Util.h"

#include <SFML\Graphics.hpp>
#include <vector>

class NodeList
{
	friend struct DungeonTile;
public:
	NodeList(int width, int height);
	~NodeList();

	void Draw(sf::RenderWindow& window, float timeDelta);

	void SetOffset(sf::Vector2f offset);
	const sf::Vector2f& GetOffset();

	DungeonTile* GetTile(const sf::Vector2f& position);
	DungeonTile* GetTile(const sf::Vector2i& coord);
	DungeonTile* GetTile(int x, int y);
	DungeonTile* GetTile(int onedCoord);

	/**
	* Resets the A* data of all level tiles.
	*/
	void ResetNodes();

	bool IsValidCoord(int x, int y);

	bool ContainsTypeAtCoord(int x, int y, DungeonTileType type);

	const int GetWidth() const;
	const int GetHeight() const;

private:
	std::vector<DungeonTile> m_grid;
	sf::Vector2f m_offset;
	int m_width;
	int m_height;
};

