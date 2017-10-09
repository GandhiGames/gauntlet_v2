#pragma once

#include "C_Updateable.h"
#include "C_Transform.h"
#include "C_Velocity.h"
#include "Raycast.h"

class C_Pathfinding : public Component, public C_Updateable
{
	friend class Level;
	friend struct Tile;
public:
	C_Pathfinding(Object* owner);
	~C_Pathfinding();

	void Update(float deltaTime) override;

private:
	sf::Vector2f* GetNextPosition();

	sf::Vector2f* GetNextReactivePosition(SharedContext& context, const sf::Vector2f& pos);

	void RemoveFirst();

	bool FindLocalPath(const sf::Vector2f& pos, const sf::Vector2f& playerPos);

	bool FindStraightPath(SharedContext& context, const sf::Vector2f& from, const sf::Vector2f& to);

	/**
	* The target position of the enemy.
	*/
	std::vector<sf::Vector2f> m_targetPositions;

private:
	std::shared_ptr<C_Velocity> m_movement;

	/**
	* The current target of the enemy.
	*/
	sf::Vector2f m_currentTarget;

	int m_speed;

	Tile* m_prevTargetTile;

	static const sf::Vector2f zeroVector;
};

