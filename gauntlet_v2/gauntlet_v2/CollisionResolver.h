#pragma once

#include <vector>
#include <memory>
#include <set>

#include "Object.h"

class CollisionResolver
{
public:
	CollisionResolver();
	~CollisionResolver();

	void UpdateCollidables(std::vector<std::shared_ptr<Object>>& objects);
	void Resolve();

	std::map<CollisionLayer, Bitmask> COLLISION_LAYERS;

	void ProcessCollisions(std::vector<std::shared_ptr<Object>>& first, std::vector<std::shared_ptr<Object>>& second);

	void ProcessRemovals();

private:
	void ProcessCollisionExits();
	void ProcessCollisionExitDueToRemovals();

private:

	std::map<CollisionLayer, std::vector<std::shared_ptr<Object>>> m_collidables;

	std::set<std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>> m_colliding;

	
};

