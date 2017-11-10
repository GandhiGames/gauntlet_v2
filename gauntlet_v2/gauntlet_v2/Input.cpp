#include "Input.h"

bool Input::IsKeyPressed(KEY keycode)
{
	switch (keycode)
	{
	case Input::KEY::KEY_LEFT:
	{
		return ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -40));
	}
	case Input::KEY::KEY_RIGHT:
	{
		return ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 40));
	}
	case Input::KEY::KEY_UP:
	{
		return ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -40));
	}
	case Input::KEY::KEY_DOWN:
	{
		return ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 40));
	}
	case Input::KEY::KEY_ATTACK:
	{
		return ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)));
	}
	case Input::KEY::KEY_ESC:
	{
		return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
	}
	case Input::KEY::KEY_RETURN:
	{
		return sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
	}
	}

	return false;
}