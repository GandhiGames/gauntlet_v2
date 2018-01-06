#include "FPS.h"

FPS::FPS() : m_timeLeft(0.f), m_accum(0.f), m_frames(0), m_updateInterval(0.2f), m_fps(0)
{
	//TODO: create generic resource manager that includes fonts
	m_font.loadFromFile("../resources/fonts/Gisha.ttf");
}

void FPS::Draw(sf::RenderWindow &window, float timeDelta)
{
	m_timeLeft -= timeDelta;
	m_accum += 1 / timeDelta;
	m_frames++;

	if (m_timeLeft <= 0.0f)
	{
		m_fps = m_accum / m_frames;

		m_timeLeft = m_updateInterval;
		m_accum = 0.0f;
		m_frames = 0;
	}

	std::string textString = std::to_string((int)m_fps);
	sf::Text text(textString, m_font);
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Red);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width, 0.f);
	const sf::View& view = window.getView();
	sf::Vector2f size = view.getSize();
	size.y *= -1;
	const sf::Vector2f pos = view.getCenter() + (size * 0.49f);
	text.setPosition(pos);

	window.draw(text);
}