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

	/**
	* Checks if a given key is currently pressed.
	* @param keycode The key to check.
	* @return True if the given key is currently pressed.
	*/
	static bool IsKeyPressed(KEY keycode);
};

