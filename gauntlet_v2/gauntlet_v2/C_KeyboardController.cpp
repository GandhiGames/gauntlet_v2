#include "C_KeyboardController.h"
#include "Object.h"

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


	m_movement->Set(movementSpeed);
}
