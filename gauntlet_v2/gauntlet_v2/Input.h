#pragma once

#include <SFML\Graphics.hpp>

class Input
{
public:

	/**
	* An enum denoting all possible input keys.
	*/
	enum class KEY
	{
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_ATTACK,
		KEY_ESC,
		KEY_RETURN
	};

	static void Initialise();

	static void EarlyUpdate();
	static void LateUpdate();

	static bool IsKeyPressed(KEY keycode);
	static bool IsKeyDown(KEY keycode);
	static bool IsKeyUp(KEY keycode);

private:
	static void SetPressedKeys();
	static std::map<KEY, bool> m_thisFrame;
	static std::map<KEY, bool> m_lastFrame;
};

