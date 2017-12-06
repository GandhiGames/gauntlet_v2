#include "C_KeyboardController.h"
#include "Object.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner), m_speed(1)
{
}


void C_KeyboardController::Awake()
{
	m_movement = m_owner->GetComponent<C_Velocity>();
}

void C_KeyboardController::Update(float timeDelta)
{
	// Calculate movement speed based on the timeDelta since the last update.
	sf::Vector2f movementSpeed(0.f, 0.f);

	if (Input::IsKeyPressed(Input::KEY::KEY_LEFT))
	{
		movementSpeed.x = -m_speed * timeDelta;;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RIGHT))
	{
		movementSpeed.x = m_speed * timeDelta;;
	}

	if (Input::IsKeyPressed(Input::KEY::KEY_UP))
	{
		movementSpeed.y = -m_speed * timeDelta;;
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_DOWN))
	{
		movementSpeed.y = m_speed * timeDelta;;
	}

	m_movement->Set(movementSpeed);
}

void C_KeyboardController::SetMovementSpeed(int speed)
{
	m_speed = speed;
}