#include "Animation.h"

Animation::Animation(sf::Texture& texture, int row, int width, int height, int frameStart, int frameEnd, int frameSpeed) :
	m_row(row),
	m_width(width),
	m_height(height),
	m_frameCountStart(frameStart),
	m_frameCountEnd(frameEnd),
	m_frameSpeed(frameSpeed),
	m_currentFrame(m_frameCountStart),
	m_timeDelta(0.f)
{
	m_sprite.setTexture(texture);

	m_sprite.setOrigin(m_width / 2.f, m_height / 2.f);

	Reset();
}


Animation::~Animation()
{
}

void Animation::Draw(sf::RenderWindow &window, float timeDelta)
{

	// add the elapsed time since the last draw call to the aggregate
	m_timeDelta += timeDelta;

	// check if the frame should be updated
	if (m_timeDelta >= (1.f / m_frameSpeed))
	{
		NextFrame();
		m_timeDelta = 0;
	}

	window.draw(m_sprite);
}

void Animation::NextFrame()
{
	// check if we reached the last frame
	if (m_currentFrame == (m_frameCountEnd - 1))
	{
		m_currentFrame = m_frameCountStart;
	}
	else
	{
		m_currentFrame++;
	}

	// update the texture rect
	m_sprite.setTextureRect(sf::IntRect(m_width * m_currentFrame, m_row * m_height, m_width, m_height));
}

void Animation::Reset()
{
	m_timeDelta = 0.f;
	m_currentFrame = m_frameCountStart;
	m_sprite.setTextureRect(sf::IntRect(0, m_row * m_height, m_width, m_height));
}
