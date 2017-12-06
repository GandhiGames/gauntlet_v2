#include "Input.h"

std::map<Input::KEY, bool> Input::m_thisFrame;
std::map<Input::KEY, bool> Input::m_lastFrame;

//TODO: implement a better solution. Do we really need two data structures?
void Input::Initialise()
{
	m_thisFrame.emplace(std::make_pair(KEY::KEY_LEFT, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_RIGHT, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_UP, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_DOWN, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_ATTACK, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_ESC, false));
	m_thisFrame.emplace(std::make_pair(KEY::KEY_RETURN, false));
}

void Input::EarlyUpdate()
{
	SetPressedKeys();
}

void Input::LateUpdate()
{
	m_lastFrame = m_thisFrame;
}

bool Input::IsKeyDown(KEY keycode)
{
	if (!IsKeyPressed(keycode))
	{
		return false;
	}

	auto key = m_lastFrame.find(keycode);

	if (key != m_lastFrame.end())
	{
		return !key->second;
	}
}

bool Input::IsKeyPressed(KEY keycode)
{
	auto key = m_thisFrame.find(keycode);

	if (key != m_thisFrame.end())
	{
		return key->second;
	}

	return false;
}

bool Input::IsKeyUp(KEY keycode)
{
	if (IsKeyPressed(keycode))
	{
		return false;
	}

	auto key = m_lastFrame.find(keycode);

	if (key != m_lastFrame.end())
	{
		return key->second;
	}

	return false;
}

void Input::SetPressedKeys()
{
	m_thisFrame[Input::KEY::KEY_LEFT] = ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -40));

	m_thisFrame[Input::KEY::KEY_RIGHT] = ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 40));

	m_thisFrame[Input::KEY::KEY_UP] = ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -40));

	m_thisFrame[Input::KEY::KEY_DOWN] = ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 40));

	m_thisFrame[Input::KEY::KEY_ATTACK] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);

	m_thisFrame[Input::KEY::KEY_ESC] = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	m_thisFrame[Input::KEY::KEY_RETURN] = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
}