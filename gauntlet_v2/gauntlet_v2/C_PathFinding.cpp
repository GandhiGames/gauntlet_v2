#include "C_Pathfinding.h"
#include "Level.h"
#include "Object.h"
#include "PathFinder.h"
#include "SharedContext.h"
#include "S_Game.h"

C_Pathfinding::C_Pathfinding(Object* owner) : Component(owner),
m_currentTarget({ 0.f, 0.f }),
m_speed(0),
m_prevTargetTile(nullptr)
{
	m_speed = rand() % 51 + 150;

	m_movement = owner->GetComponent<C_Velocity>();
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

	auto pos = m_owner->m_transform->GetPosition();

	if (curTile != m_prevTargetTile)
	{
		m_prevTargetTile = curTile;

		if (Mathf::distance(playerPos, pos) < 300.0f)
		{
			// Need to find path to player.
			if (m_owner->m_context.m_pathFinder->IsCached(pos, playerPos))
			{
				// If path is cached then lets retrieve that and not bother with other optomisation attempts.
				m_targetPositions = context.m_pathFinder->GetPath(pos, playerPos);
			}
			else
			{
				// If the player is in sight then head directly there.
				bool foundStraightPath = FindStraightPath(context, pos, playerPos); 

				if (!foundStraightPath)
				{
					// Do any other nearby enemies have an up-to-date path to the player that we can borrow?
					bool foundLocalPath = FindLocalPath(pos, playerPos);

					if (!foundLocalPath)
					{
						// If all else fails lets calculate the path.
						m_targetPositions = context.m_pathFinder->GetPath(pos, playerPos);
					}
				}
			}

		}
	}

	sf::Vector2f* targetLocation = GetNextReactivePosition(context, pos);

	// Move towards current target location.
	if (targetLocation)
	{
		sf::Vector2f velocity = sf::Vector2f(targetLocation->x - pos.x, targetLocation->y - pos.y);

		if (abs(velocity.x) < 10.f && abs(velocity.y) < 10.f) // Reached target.
		{
			if (m_targetPositions.size() == 1)
			{
				m_targetPositions.clear();
			}
			//RemoveFirst();
		}
		else
		{
			sf::Vector2f velNorm = Mathf::normalize(velocity);

			m_movement->Set((velNorm * (float)m_speed * deltaTime));
		}
	}
	else
	{
		m_movement->Set(sf::Vector2f(0.f, 0.f));
	}
}

bool C_Pathfinding::FindStraightPath(SharedContext& context, const sf::Vector2f& from, const sf::Vector2f& to)
{
	RaycastResult result = Raycast::Cast(context, from, to);

	if (result.collision)
	{
		return false;
	}
	else
	{
		//Debug::Log("\nMoving directly to target");
		m_targetPositions.clear();
		m_targetPositions.push_back(to);
		return true;
	}
}

bool C_Pathfinding::FindLocalPath(const sf::Vector2f& pos, const sf::Vector2f& playerPos)
{
	bool foundLocal = false;


	auto others = Object::GetObjectsWithTag(FOLLOWER_TAG);
	for (const auto& o : others)
	{
		if (Mathf::distance(pos, o->m_transform->GetPosition()) < 50.f)
		{
			auto pathFinding = o->GetComponent<C_Pathfinding>();

			if (pathFinding->m_targetPositions.size() > 0)
			{
				if (Mathf::distance(pathFinding->m_targetPositions.back(), playerPos) < 20.f)
				{
					m_targetPositions = pathFinding->m_targetPositions;
					foundLocal = true;
					break;
				}
			}
		}

	}


	return foundLocal;

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

sf::Vector2f* C_Pathfinding::GetNextReactivePosition(SharedContext& context, const sf::Vector2f& pos)
{
	sf::Vector2f* target = nullptr;

	if (!m_targetPositions.empty())
	{
		auto itr = m_targetPositions.end();
		while (itr > m_targetPositions.begin())
		{
			--itr;

			if (target)
			{
				itr = m_targetPositions.erase(itr);
			}
			else
			{
				auto v = *itr;

				if (!Raycast::Cast(context, pos, v).collision) //If there is no obstacle between the entity and that position.
				{
				//	Debug::Log("\n Getting Reactive position");
					target = &v;
				}
			}

		}

		// Cannot reach any target positions so remove them.
		if (!target)
		{
			m_targetPositions.clear();
		}
	}

	return target;
}


void C_Pathfinding::RemoveFirst()
{
	if (!m_targetPositions.empty())
	{
		m_targetPositions.erase(m_targetPositions.begin());
	}
}
