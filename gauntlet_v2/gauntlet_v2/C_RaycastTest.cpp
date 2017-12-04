#include "C_RaycastTest.h"
#include "Object.h"

C_RaycastTest::C_RaycastTest(Object* owner) : Component(owner)
{
	//toOffset = sf::Vector2f(rand() % 100 + (-50), rand() % 100 + (-50));
	m_toOffset = sf::Vector2f(0.f, -50.f);
}

void C_RaycastTest::Update(float deltaTime)
{
	auto from = m_owner->m_transform->GetPosition();
	auto to = from + m_toOffset; 

	RaycastResult hit = Raycast::Cast(from, to);

	if (hit.collision)
	{
		Debug::DrawRay(from, to, sf::Color::Red);
	}
	else
	{
		Debug::DrawRay(from, to, sf::Color::White);
	}
}