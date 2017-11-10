#pragma once

#include <string>
#include <SFML\Graphics.hpp>

class Animation
{
public:
	Animation(sf::Texture& texture, int row, int width, int height, int frameStart, int frameEnd, int frameSpeed);
	~Animation();

	void Draw(sf::RenderWindow &window, float timeDelta);

	void NextFrame();

	void Reset();

	sf::Sprite m_sprite;

	float m_timeDelta;
	int m_row;
	int m_width;
	int m_height;
	int m_frameCountStart;
	int m_frameCountEnd;
	int m_frameSpeed;
	int m_currentFrame;
};

