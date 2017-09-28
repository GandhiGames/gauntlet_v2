#pragma once

#include "Input.h"
#include "S_Game.h"
#include "SharedContext.h"
#include "StateManager.h"

class Game
{
public:
	Game();
	~Game();

	void Update();
	void Draw();
	void LateUpdate();

	bool IsRunning();

private:
	sf::RenderWindow m_window;
	SharedContext m_context;
	StateManager m_stateManager;

	bool m_isRunning;

	sf::Clock m_clock;
	float m_elapsed;

	
};

