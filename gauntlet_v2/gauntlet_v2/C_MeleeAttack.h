#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "C_Direction.h"
#include "C_AnimatedSprite.h"
#include "Input.h"
#include "Raycast.h"

class C_MeleeAttack : public Component, public C_Updateable
{
public:
	C_MeleeAttack(Object * owner);

	void Awake() override;
	void Start() override;

	void DoMeleeAttack();

	void Update(float deltaTime) override;

private:
	std::shared_ptr<C_Direction> m_moveDir;
	std::map<MOVEMENT_DIRECTION, sf::Vector2f> m_hitDirections;
	std::shared_ptr<C_AnimatedSprite> m_animation;
	float hitDistance;
	float hitRadius;
	int dmgAmount;
};

