#include "C_MeleeAttack.h"
#include "Object.h"
#include "Debug.h"

C_MeleeAttack::C_MeleeAttack(Object * owner) : Component(owner)
{
}

void C_MeleeAttack::Awake()
{
	m_moveDir = m_owner->GetComponent<C_Direction>();
	m_animation = m_owner->GetComponent<C_AnimatedSprite>();
}

void C_MeleeAttack::Start()
{
	m_attackDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, ANIMATION_STATE::SWING_LEFT));
	m_attackDirections.insert(std::make_pair(MOVEMENT_DIRECTION::DOWN, ANIMATION_STATE::SWING_DOWN));
	m_attackDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, ANIMATION_STATE::SWING_RIGHT));
	m_attackDirections.insert(std::make_pair(MOVEMENT_DIRECTION::UP, ANIMATION_STATE::SWING_UP));
}

void C_MeleeAttack::Update(float deltaTime)
{
	if (Input::IsKeyUp(Input::KEY::KEY_ATTACK))
	{
		const MOVEMENT_DIRECTION moveDir = m_moveDir->Get();

		auto anim = m_attackDirections.find(moveDir);

		if (anim != m_attackDirections.end())
		{
			m_animation->SetCurrentAnimation(anim->second);
		}

		const sf::Vector2f& pos = m_owner->m_transform->GetPosition();
		auto inRange = Raycast::CircleCast(pos, 10.f, FOLLOWER_TAG);

		for (auto i : inRange)
		{
			i->Destroy();
		}
	}
}