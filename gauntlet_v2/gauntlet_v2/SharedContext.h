#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Object.h"

class SharedContext
{
	friend class PathFinder;
	friend class S_Game;
public:
	sf::RenderWindow* m_window;
	Level* m_level;
	std::shared_ptr<Object>* m_player;
	PathFinder* m_pathFinder;
};