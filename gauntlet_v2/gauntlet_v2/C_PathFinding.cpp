#include "C_Pathfinding.h"
#include "Level.h"
#include "Object.h"
#include "PathFinder.h"

C_Pathfinding::C_Pathfinding(Object* owner) : Component(owner),
m_currentTarget({ 0.f, 0.f }),
m_speed(0),
m_prevTargetTile(nullptr)
{
	m_speed = rand() % 51 + 150;

	m_movement = owner->GetComponent<C_Movement>();
}


C_Pathfinding::~C_Pathfinding()
{
}

void C_Pathfinding::Update(float deltaTime)
{
	SharedContext& context = m_owner->m_context;
	const sf::Vector2f& playerPos = (*context.m_player)->m_transform->GetPosition();
	Level* level = context.m_level;

	Tile*curTile = level->GetTile(playerPos);

	if (curTile != m_prevTargetTile)
	{
		m_prevTargetTile = curTile;

		bool foundLocalPath = false;

		auto pos = m_owner->m_transform->GetPosition();
		if (DistanceBetweenPoints(playerPos, pos) < 300.0f)
		{
			if (!context.m_pathFinder->IsCached(pos, playerPos))
			{
				auto others = m_owner->m_context.m_objects;
				for (const auto& o : *others)
				{
					if (o->m_tag->Get() == FOLLOWER_TAG && DistanceBetweenPoints(pos, o->m_transform->GetPosition()) < 50.f)
					{
						auto pathFinding = o->GetComponent<C_Pathfinding>();

						if (pathFinding->m_targetPositions.size() > 0)
						{
							if (DistanceBetweenPoints(pathFinding->m_targetPositions.back(), playerPos) < 20.f)
							{
								m_targetPositions = pathFinding->m_targetPositions;
								foundLocalPath = true;
								break;
							}
						}
					}
				}
			}

			if (!foundLocalPath)
			{
				m_targetPositions = context.m_pathFinder->GetPath(pos, playerPos);
			}
		}
	}

	sf::Vector2f* targetLocation = GetNextPosition();

	// Move towards current target location.
	if (targetLocation)
	{
		sf::Vector2f velocity = sf::Vector2f(targetLocation->x - m_owner->m_transform->GetPosition().x,
			targetLocation->y - m_owner->m_transform->GetPosition().y);

		if (abs(velocity.x) < 10.f && abs(velocity.y) < 10.f) // Reached target.
		{
			RemoveFirst();
		}
		else
		{
			float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
			velocity.x /= length;
			velocity.y /= length;

			m_movement->SetVelocity((velocity * (float)m_speed * deltaTime));
		}
	}
	else
	{
		m_movement->SetVelocity(sf::Vector2f(0.f, 0.f));
	}
}

//TODO: move to last visible position (similiar to left 4 dead ai)


sf::Vector2f* C_Pathfinding::GetNextPosition()
{
	if (!m_targetPositions.empty())
	{
		return &m_targetPositions.front();
	}

	return nullptr;
}


void C_Pathfinding::RemoveFirst()
{
	if (!m_targetPositions.empty())
	{
		m_targetPositions.erase(m_targetPositions.begin());
	}
}
