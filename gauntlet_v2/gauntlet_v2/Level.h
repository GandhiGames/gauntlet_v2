#pragma once
#include "SharedContext.h"

//TODO: Remove this class!

// Constants for the game grid size.
static int const GRID_WIDTH = 37;
static int const GRID_HEIGHT = 21;
static int const ROOM_WIDTH_MAX = 7;
static int const ROOM_HEIGHT_MAX = 7;
static int const ROOM_COUNT_MAX = 15;

// The width and height of each tile in pixels.
static int const TILE_SIZE = 16;

enum class TILE 
{
	WALL_SINGLE,
	WALL_TOP_END,
	WALL_SIDE_RIGHT_END,
	WALL_BOTTOM_LEFT,
	WALL_BOTTOM_END,
	WALL_SIDE,
	WALL_TOP_LEFT,
	WALL_SIDE_LEFT_T,
	WALL_SIDE_LEFT_END,
	WALL_BOTTOM_RIGHT,
	WALL_TOP,
	WALL_BOTTOM_T,
	WALL_TOP_RIGHT,
	WALL_SIDE_RIGHT_T,
	WALL_TOP_T,
	WALL_INTERSECTION,
	WALL_DOOR_LOCKED,
	WALL_DOOR_UNLOCKED,
	WALL_ENTRANCE,
	FLOOR,
	FLOOR_ALT,
	EMPTY,
	COUNT
};


// The level tile type.
struct Tile 
{
	TILE type;							// The type of tile this is.
	int x;					// The column index of the tile.
	int y;						// The row index of the tile.
	sf::Sprite sprite;					// The tile sprite.
	int H;								// Heuristic / movement cost to goal.
	int G;								// Movement cost. (Total of entire path)
	int F;								// Estimated cost for full path. (G + H)
	Tile* parentNode;					// Node to reach this node.
};

class Level
{
	friend class Object;

public:
	/**
	* Default constructor.
	*/
	Level();

	/**
	* Constructor.
	* A renderWindow is needed in order for the level to calculate its position.
	* @param window The game window.
	*/
	Level(SharedContext* context);

	/**
	* Returns true if the given tile index is solid.
	* @param columnIndex The tile's column index.
	* @param rowIndex The tile's row index.
	* @return True if the given tile is solid.
	*/
	bool IsSolid(int x, int y);
	bool IsSolid(sf::Vector2f pos);

	/**
	* Sets the index of a given tile in the 2D game grid.
	* This also changes the tile sprite, and is how tiles should be changed and set manually.
	* @param columnIndex The tile's column index.
	* @param rowIndex The tile's row index.
	* @param index The new index of the tile.
	*/
	void SetTile(int x, int y, TILE tileType);

	/**
	* Draws the level grid to the provided render window.
	* @param window The render window to draw the level to.
	* @param timeDelta The time that has elapsed since the last update.
	*/
	void Draw(sf::RenderWindow &window, float timeDelta);

	/**
	* Gets the index of the given tile.
	* @param columnIndex The column index of the tile to check.
	* @param rowIndex The row index of the tile to check.
	* @return The index of the given tile.
	*/
	TILE GetTileType(int x, int y) const;

	/**
	* Gets the tile at the given position.
	* @param position The coordinates of the position to check.
	* @return A pointer to the tile at the given location.
	*/
	Tile* GetTile(sf::Vector2f position);

	/**
	* Gets the tile at the given position in the level array.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The row that the tile is in.
	* @return A pointer to the tile if valid.
	*/
	Tile* GetTile(int x, int y);

	/**
	* Gets the position of the level grid relative to the window.
	* @return The position of the top-left of the level grid.
	*/
	sf::Vector2f GetPosition() const;

	/**
	* Checks if a given tile is valid.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The column that the row is in.
	* @return True if the tile is valid.
	*/
	bool TileIsValid(int x, int y);

	/**
	* Sets the overlay color of the level tiles.
	* @param tileColor The new tile overlay color.
	*/
	void SetColor(sf::Color tileColor);

	/**
	* Gets the size of the level in terms of tiles.
	* @return The size of the level grid.
	*/
	sf::Vector2i GetSize() const;

	/**
	* Gets the actual position of a tile in the game.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The column that the row is in.
	* @return The position of the tile if valid.
	*/
	sf::Vector2f GetActualTileLocation(int x, int y);

	/**
	* Returns a valid spawn location from the current room.
	* The position returned is relative to the game window.
	* @return A suitable spawn location within the room.
	*/
	sf::Vector2f GetRandomSpawnLocation();

	/**
	* Resets the A* data of all level tiles.
	*/
	void ResetNodes();

	/**
	* Generates a random level.
	*/
	void GenerateLevel();

	/**
	* Spawns a given number of torches in the level.
	* @param torchCount The number of torches to create.
	*/
	//void SpawnTorches(int torchCount);

	/**
	* Returns the spawn location for the current level.
	* @return The spawn location of the level.
	*/
	sf::Vector2f GetSpawnLocation() const;

	/**
	* Unlocks the door in the level.
	*/
	void UnlockDoor();

	/**
	* Return true if the given tile is a floor tile.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The column that the row is in.
	* @return True if the given tile is a floor tile.
	*/
	bool IsFloor(int x, int y);

	/**
	* Return true if the given tile is a floor tile.
	* @param tile The tile to check
	* @return True if the given tile is a floor tile.
	*/
	bool IsFloor(const Tile& tile);

	/**
	* Returns the size of the tiles in the level.
	* @return The size of the tiles in the level.
	*/
	int GetTileSize() const;

	bool CausesCollision(const sf::Vector2f& pos);

private:
	void BuildSprite(Tile &t);

	/**
	* Creates a path between two nodes in the recursive backtracker algorithm.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The column that the row is in.
	*/
	void CreatePath(int x, int y);

	/**
	* Adds a given number of randomly sized rooms to the level to create some open space.
	*/
	void CreateRooms(int roomCount);

	/**
	* Calculates the correct texture for each tile in the level.
	*/
	void CalculateTextures();

	/**
	* Generates an entry and exit point for the given level.
	*/
	void GenerateEntryExit();

	/**
	* Checks if a given tile is a wall block.
	* @param columnIndex The column that the tile is in.
	* @param rowIndex The column that the row is in.
	* @return True if the given tile is a wall tile.
	*/
	bool IsWall(int x, int y);

private:

	SharedContext* m_context;

	/**
	* A 2D array that describes the level data.
	* The type is Tile, which holds a sprite and an index.
	*/
	Tile m_grid[GRID_WIDTH][GRID_HEIGHT];

	/**
	* The position of the level relative to the window.
	* This is to the top-left of the level grid.
	*/
	sf::Vector2i m_origin;

	int m_textureID;

	/**
	* The spawn location for the current level.
	*/
	sf::Vector2f m_spawnLocation;

	/**
	* The indices of the tile containing the levels door.
	*/
	sf::Vector2i m_doorTileIndices;

	std::map<TILE, sf::IntRect> m_spriteLookup;
};
