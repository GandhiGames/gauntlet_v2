#include "C_AnimatedSprite.h"

#include "Object.h"

C_AnimatedSprite::C_AnimatedSprite(Object* owner) : Component(owner),
m_animationSpeed(0),
m_frameCount(0),
m_currentFrame(0),
m_frameWidth(0),
m_frameHeight(0),
m_timeDelta(0),
m_animated(false)
{
	//TODO: test this works, implement more error checking.
	assert(owner->GetComponents<C_Drawable>().size() == 0);
}

C_AnimatedSprite::~C_AnimatedSprite()
{
}

void C_AnimatedSprite::Update(float deltaTime)
{
	m_sprite.setPosition(m_owner->m_transform->GetPosition());
}

void C_AnimatedSprite::SetAnimated(bool animated)
{
	m_animated = animated;

	if (m_animated)
	{
		m_currentFrame = 0;
	}
	else
	{
		// set the texture rect of the first frame
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_frameWidth, m_frameHeight));
	}
}

bool C_AnimatedSprite::IsAnimated()
{
	return m_animated;
}

// Gives the object the given sprite.
bool C_AnimatedSprite::SetSprite(sf::Texture& texture, bool isSmooth, int frames, int frameSpeed)
{
	// Create a sprite from the loaded texture.
	m_sprite.setTexture(texture);

	// Set animation speed.
	m_animationSpeed = frameSpeed;

	// Store the number of frames.
	m_frameCount = frames;

	// Calculate frame dimensions.
	sf::Vector2u texSize = m_sprite.getTexture()->getSize();
	m_frameWidth = texSize.x / m_frameCount;
	m_frameHeight = texSize.y;

	// Check if animated or static.
	if (frames > 1)
	{
		// Set sprite as animated.
		m_animated = true;

		// Set the texture rect of the first frame.
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_frameWidth, m_frameHeight));
	}
	else
	{
		m_animated = false;
	}

	// Set the origin of the sprite.
	m_sprite.setOrigin(m_frameWidth / 2.f, m_frameHeight / 2.f);

	return true;
}

//TODO: remove this function and provide functionality through different methods.
const sf::Sprite& C_AnimatedSprite::GetSprite()
{
	return m_sprite;
}

void C_AnimatedSprite::SetOrigin(const sf::Vector2f origin)
{
	m_sprite.setOrigin(origin);
}

void C_AnimatedSprite::SetTexture(const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

// Draws the object to the given render window.
void C_AnimatedSprite::Draw(sf::RenderWindow &window, float timeDelta)
{
	if (m_animated)
	{
		// add the elapsed time since the last draw call to the aggregate
		m_timeDelta += timeDelta;

		// check if the frame should be updated
		if (m_timeDelta >= (1.f / m_animationSpeed))
		{
			NextFrame();
			m_timeDelta = 0;
		}
	}

	window.draw(m_sprite);
}

// Advances the sprite forward a frame.
void C_AnimatedSprite::NextFrame()
{
	// check if we reached the last frame
	if (m_currentFrame == (m_frameCount - 1))
	{
		m_currentFrame = 0;
	}
	else
	{
		m_currentFrame++;
	}

	// update the texture rect
	m_sprite.setTextureRect(sf::IntRect(m_frameWidth * m_currentFrame, 0, m_frameWidth, m_frameHeight));
}

// Gets the frame count of the object.
int C_AnimatedSprite::GetFrameCount() const
{
	return m_frameCount;
}