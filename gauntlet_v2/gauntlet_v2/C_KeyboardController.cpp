#include "C_KeyboardController.h"
#include "Object.h"
#include "Level.h"
#include "SharedContext.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner)
{
	m_speed = 200;

	m_movement = owner->GetComponent<C_Velocity>();
}


C_KeyboardController::~C_KeyboardController()
{
}

void C_KeyboardController::Update(float timeDelta)
{
	// Calculate movement speed based on the timeDelta since the last update.
	sf::Vector2f movementSpeed(0.f, 0.f);
	sf::Vector2f previousPosition = m_owner->m_transform->GetPosition();

	if (Input::IsKeyPressed(Input::KEY::KEY_LEFT))
	{
		// Set movement speed.
		movementSpeed.x = -m_speed * timeDelta;;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RIGHT))
	{
		// Set movement speed.
		movementSpeed.x = m_speed * timeDelta;;
	}

	if (Input::IsKeyPressed(Input::KEY::KEY_UP))
	{
		// Set movement speed.
		movementSpeed.y = -m_speed * timeDelta;;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_DOWN))
	{
		// Set movement speed.
		movementSpeed.y = m_speed * timeDelta;;
	}


	Level* level = m_owner->m_context.m_level;

	// Calculate horizontal movement.
	if (CausesCollision(sf::Vector2f(movementSpeed.x, 0.0f), *level))
	{
		movementSpeed.x = 0.f;
	}

	// Calculate horizontal movement.
	if (CausesCollision(sf::Vector2f(0.0f, movementSpeed.y), *level))
	{
		movementSpeed.y = 0.f;
	}


	m_movement->Set(movementSpeed);
}


bool C_KeyboardController::CausesCollision(const sf::Vector2f& movement, Level& level)
{
	// Get the tiles that the four corners other player are overlapping with.
	Tile* overlappingTiles[4];
	sf::Vector2f newPosition = m_owner->m_transform->GetPosition() + movement;

	// Top left.
	overlappingTiles[0] = level.GetTile(sf::Vector2f(newPosition.x - 14.f, newPosition.y - 14.f));

	// Top right.
	overlappingTiles[1] = level.GetTile(sf::Vector2f(newPosition.x + 14.f, newPosition.y - 14.f));

	// Bottom left.
	overlappingTiles[2] = level.GetTile(sf::Vector2f(newPosition.x - 14.f, newPosition.y + 14.f));

	// Bottom right.
	overlappingTiles[3] = level.GetTile(sf::Vector2f(newPosition.x + 14.f, newPosition.y + 14.f));

	// If any of the overlapping tiles are solid there was a collision.
	for (int i = 0; i < 4; i++)
	{
		if (level.IsSolid(overlappingTiles[i]->columnIndex, overlappingTiles[i]->rowIndex))
			return true;
	}

	// If we've not returned yet no collisions were found.
	return false;
}

