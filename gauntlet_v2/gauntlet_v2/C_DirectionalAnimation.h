#pragma once

#include<tuple>
#include<array>

#include "Component.h"
#include "C_Updateable.h"
#include "C_AnimatedSprite.h"
#include "C_Velocity.h"
#include "Util.h"
#include "TextureManager.h"


class C_DirectionalAnimation : public Component, public C_Updateable
{
	friend class Object;

public:
	C_DirectionalAnimation(Object* owner);
	~C_DirectionalAnimation();

	void Update(float timeDelta) override;


private:
	std::shared_ptr<C_AnimatedSprite> m_sprite;
	std::shared_ptr<C_Velocity> m_movement;

	ANIMATION_STATE m_currentState;

	MOVEMENT_DIRECTION m_prevDirection;

	std::map<MOVEMENT_DIRECTION, ANIMATION_STATE> m_idleDirections;
};

