#pragma once

#include<tuple>
#include<array>

#include "Component.h"
#include "C_Updateable.h"
#include "C_AnimatedSprite.h"
#include "C_Velocity.h"
#include "C_Direction.h"
#include "Util.h"
#include "TextureManager.h"


class C_DirectionalAnimation : public Component, public C_Updateable
{
	friend class Object;

public:
	C_DirectionalAnimation(Object* owner);

	void Awake() override;

	void Update(float timeDelta) override;


private:
	std::shared_ptr<C_AnimatedSprite> m_sprite;
	std::shared_ptr<C_Velocity> m_movement;
	std::shared_ptr<C_Direction> m_direction;

	ANIMATION_STATE m_currentState;
};

