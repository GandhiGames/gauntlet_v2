#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Object.h"

class SharedContext
{
	friend class PathFinder;
public:
	sf::RenderWindow* m_window;
	std::vector<std::shared_ptr<Object>>* m_objects;
	std::vector<std::shared_ptr<Object>>* m_newObjects;
	Level* m_level;
	std::shared_ptr<Object>* m_player;
	PathFinder* m_pathFinder;
};