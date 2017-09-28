#pragma once

#include<tuple>
#include<array>

#include "Component.h"
#include "C_Updateable.h"
#include "C_AnimatedSprite.h"
#include "C_Movement.h"
#include "Util.h"
#include "TextureManager.h"


class C_DirectionalAnimation : public Component, public C_Updateable
{
	friend class Object;

public:
	C_DirectionalAnimation(Object* owner);
	~C_DirectionalAnimation();

	void Update(float timeDelta) override;

	//TODO: performance concerns with passing std::array?
	void SetTextures(std::array<int, static_cast<int>(ANIMATION_STATE::COUNT)> textureIDs);

	const int& GetTextureID() const;

	//TODO: getters/setters
	std::array<int, static_cast<int>(ANIMATION_STATE::COUNT)> m_textureIDs;
	std::array<sf::Texture, static_cast<int>(ANIMATION_STATE::COUNT)> m_textures;

private:
	std::shared_ptr<C_AnimatedSprite> m_sprite;
	std::shared_ptr<C_Movement> m_movement;


	int m_currentTextureIndex;
};

