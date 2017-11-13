#pragma once

#include "SharedContext.h"
#include "DungeonClusters.h"
#include "TextureManager.h"
#include "Debug.h"
#include "NodeList.h"

static int const DUNGEON_WIDTH = 47;
static int const DUNGEON_HEIGHT = 47;
static int const DUNGEON_TILE_SIZE = 32;

struct DungeonTile
{
	DungeonTileType type;							// The type of tile this is.
	int x;					// The column index of the tile.
	int y;						// The row index of the tile.
	sf::Sprite sprite;					// The tile sprite.
	int H;								// Heuristic / movement cost to goal.
	int G;								// Movement cost. (Total of entire path)
	int F;								// Estimated cost for full path. (G + H)
	DungeonTile* parentNode;					// Node to reach this node.
};

class DungeonGenerator
{
public:
	DungeonGenerator(SharedContext& context);
	~DungeonGenerator();

	void Generate();

	void Draw(sf::RenderWindow& window, float timeDelta);

	/**
	* Gets the size of the level in terms of tiles.
	* @return The size of the level grid.
	*/
	sf::Vector2i GetSize() const;

	sf::Vector2f GetTilePosition(int x, int y);

	/**
	* Returns true if the given tile index is solid.
	* @param columnIndex The tile's column index.
	* @param rowIndex The tile's row index.
	* @return True if the given tile is solid.
	*/
	bool IsSolid(int x, int y);
	bool IsSolid(const sf::Vector2f& pos);
	bool IsSolid(DungeonTile& tile);

	bool CausesCollision(const sf::Vector2f& pos);

	NodeList& GetNodes();

	sf::Vector2f GetRandomFloorLocation();

private:
	void BuildSprite(DungeonTile& t);

	void InitialiseEnvironment();
	void PerformTransistionStep();
	void RemoveExtraneous();
	void RemoveLoneCells();
	void DefineWallTypes();
	void BuildFacade();
	void PlaceEntranceAndExit();
	void DisplayEnvironment();

	bool IsEdge(int x, int y);
	int CountTileWallNeighbours(int x, int y);
	bool IsExtraneousCell(int x, int y);

	bool IsLoneCell(int x, int y);
	void DefineWallType(int x, int y);

private:
	DungeonClusters m_clusters;
	
	NodeList m_nodes;

	std::map<DungeonTileType, sf::IntRect> m_spriteLookup;
	
	int m_tilesetID;

	//Dungeon setup details
	int m_transitionSteps;
	bool m_connected;
	float m_chanceToBecomeWall;
	int m_backgroundToWallConversion;
	int m_wallToBackgroundConversion;
};

