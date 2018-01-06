#include "C_MeleeAttack.h"
#include "Object.h"
#include "Debug.h"

C_MeleeAttack::C_MeleeAttack(Object * owner) : Component(owner),
hitDistance(10.f), hitRadius(20.f), dmgAmount(1)
{
}

void C_MeleeAttack::Awake()
{
	m_moveDir = m_owner->GetComponent<C_Direction>();
	m_animation = m_owner->GetComponent<C_AnimatedSprite>();
}

void C_MeleeAttack::Start()
{
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, sf::Vector2f(-hitDistance, 0.f)));
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, sf::Vector2f(hitDistance, 0.f)));
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::DOWN, sf::Vector2f(0.f, hitDistance)));
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::UP, sf::Vector2f(0.f, -hitDistance)));


	std::shared_ptr<AnimationGroup> swingAnimation = m_animation->GetAnimation(ANIMATION_STATE::SWING);

	if (swingAnimation)
	{
		auto animations = swingAnimation->GetAnimations(SPRITE_TYPE::SWORD);

		for (auto& a : animations)
		{
			a->SetFrameAction(3, std::bind(&C_MeleeAttack::DoMeleeAttack, this));
		}
	}

}

void C_MeleeAttack::Update(float deltaTime)
{
	if (Input::IsKeyDown(Input::KEY::KEY_ATTACK))
	{
		m_animation->SetCurrentAnimation(ANIMATION_STATE::SWING);
	}
}

void C_MeleeAttack::DoMeleeAttack()
{
	const MOVEMENT_DIRECTION moveDir = m_moveDir->Get();
	const sf::Vector2f& pos = m_owner->m_transform->GetPosition();
	auto inRange = Raycast::CircleCast(pos + m_hitDirections.at(moveDir), hitRadius, FOLLOWER_TAG);

	for (auto i : inRange)
	{
		const sf::Vector2f& heading = i->m_transform->GetPosition() - pos;
		const float distance = Mathf::magnitude(heading);
		const sf::Vector2f& dir = heading / distance;

		auto damageables = i->GetComponents<C_Damageable>();

		for (auto& d : damageables)
		{
			d->DoDamage(dir, dmgAmount);
		}
	}
}