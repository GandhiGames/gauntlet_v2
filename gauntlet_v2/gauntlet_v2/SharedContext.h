#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Object.h"

class SharedContext
{
	friend class PathFinder;
	friend class S_Game;
	friend class DungeonGenerator;
public:

	//SharedContext::SharedContext(sf::RenderWindow* window, DungeonGenerator* level, std::shared_ptr<Object> player,
	//	PathFinder* pathFinder) : m_window(window), m_level(level), m_player(player), m_pathFinder(pathFinder) {}

	sf::RenderWindow* m_window;
	DungeonGenerator* m_level;
	std::shared_ptr<Object> m_player;
	PathFinder* m_pathFinder;
};