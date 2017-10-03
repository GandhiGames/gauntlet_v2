#include "Game.h"


Game::Game() : m_window(sf::VideoMode::getDesktopMode(), "Gauntlet_v0.1", sf::Style::Titlebar),
m_isRunning(true), m_stateManager(&m_context)
{
	// Set a random seed.
	srand(static_cast<unsigned int>(time(nullptr)));

	m_window.setVerticalSyncEnabled(true);
	m_window.setMouseCursorVisible(false);
	m_window.setFramerateLimit(60);


	m_elapsed = m_clock.restart().asSeconds();

	m_context.m_window = &m_window;

	m_stateManager.SwitchTo(StateType::Game);
}


Game::~Game()
{
}

void Game::Update()
{
	sf::Event event;
	if (m_window.pollEvent(event))
	{
		if ((event.type == sf::Event::Closed) || (Input::IsKeyPressed(Input::KEY::KEY_ESC)))
		{
			m_isRunning = false;
			m_window.close();
			return;
		}
	}

	m_stateManager.Update(m_elapsed);
}

void Game::Draw()
{
	m_window.clear(sf::Color(0, 0, 0, 225));

	m_stateManager.Draw(m_elapsed);

	// Present the back-buffer to the screen.
	m_window.display();
}

void Game::LateUpdate()
{
	m_stateManager.LateUpdate(m_elapsed);

	m_elapsed = m_clock.restart().asSeconds();
}

bool Game::IsRunning()
{
	return m_isRunning;
}
