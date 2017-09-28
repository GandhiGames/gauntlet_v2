#include "C_Collider2D.h"

C_Collider2D::C_Collider2D() : m_isTrigger(false), m_layer(CollisionLayer::Default)
{
}


C_Collider2D::~C_Collider2D()
{
}

bool C_Collider2D::IsTrigger() const
{
	return m_isTrigger;
}

void C_Collider2D::SetTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
}

CollisionLayer C_Collider2D::GetLayer() const
{
	return m_layer;
}

void C_Collider2D::SetLayer(CollisionLayer layer)
{
	m_layer = layer;
}
