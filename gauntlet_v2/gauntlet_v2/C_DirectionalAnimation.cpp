#include "C_DirectionalAnimation.h"

#include "C_DirectionalAnimation.h"
#include "Object.h"

C_DirectionalAnimation::C_DirectionalAnimation(Object* owner) : Component(owner),
m_currentTextureIndex(static_cast<int>(ANIMATION_STATE::WALK_DOWN))
{
	m_sprite = owner->GetComponent<C_AnimatedSprite>();
	m_movement = owner->GetComponent<C_Velocity>();
}


C_DirectionalAnimation::~C_DirectionalAnimation()
{
}

void C_DirectionalAnimation::SetTextures(std::array<int, static_cast<int>(ANIMATION_STATE::COUNT)> textureIDs)
{
	//TODO: remove texture ids? are they necessary?
	m_textureIDs = textureIDs;

	//TODO: cache textures so don't need to call texturemanager::gettexture
	for (int i = 0; i < static_cast<int>(ANIMATION_STATE::COUNT); i++)
	{
		m_textures[i] = TextureManager::GetTexture(m_textureIDs[i]);
	}
}

void C_DirectionalAnimation::Update(float deltaTime)
{
	ANIMATION_STATE animState = static_cast<ANIMATION_STATE>(m_currentTextureIndex);

	const sf::Vector2f& velocity = m_movement->Get();

	if ((velocity.x != 0) || (velocity.y != 0))
	{
		if (abs(velocity.x) > abs(velocity.y))
		{
			if (velocity.x <= 0)
			{
				animState = ANIMATION_STATE::WALK_LEFT;
			}
			else
			{
				animState = ANIMATION_STATE::WALK_RIGHT;
			}
		}
		else
		{
			if (velocity.y <= 0)
			{
				animState = ANIMATION_STATE::WALK_UP;
			}
			else
			{
				animState = ANIMATION_STATE::WALK_DOWN;
			}
		}
	}

	// set animation speed
	if ((velocity.x == 0) && (velocity.y == 0))
	{
		// the character is still
		if (m_sprite->IsAnimated())
		{
			// Update sprite to idle version.
			// In our enum we have 4 walking sprites followed by 4 idle sprites.
			// Given this, we can simply add 4 to a walking sprite to get its idle counterpart.
			m_currentTextureIndex += 4;

			// Stop movement animations.
			m_sprite->SetAnimated(false);
		}
	}
	else
	{
		// the character is moving
		if (!m_sprite->IsAnimated())
		{
			// Update sprite to walking version.
			m_currentTextureIndex -= 4;

			// Start movement animations.
			m_sprite->SetAnimated(true);
		}
	}

	// Set the sprite.
	if (m_currentTextureIndex != static_cast<int>(animState))
	{
		m_currentTextureIndex = static_cast<int>(animState);

		//m_sprite->GetSprite().setTexture(m_textures[textureId]);

		//TODO: optomise out calls to GetTexture?
		m_sprite->SetTexture(m_textures[m_currentTextureIndex]);
	}
}

const int& C_DirectionalAnimation::GetTextureID() const
{
	return m_currentTextureIndex;
}