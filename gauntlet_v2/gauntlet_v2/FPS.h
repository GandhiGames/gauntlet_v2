#pragma once

#include <SFML\Graphics.hpp>

class FPS
{
public:
	FPS();
	void Draw(sf::RenderWindow &window, float timeDelta);

private:
	sf::Font m_font;
	float m_timeLeft;
	float m_accum;
	int m_frames;
	float m_updateInterval;
	float m_fps;
};

