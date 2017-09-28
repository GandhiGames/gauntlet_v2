#include "C_Drawable.h"

C_Drawable::C_Drawable() : m_sortOrder(0)
{
}


C_Drawable::~C_Drawable()
{
}

void C_Drawable::SetSortOrder(const int& order)
{
	m_sortOrder = order;
}

const int& C_Drawable::GetSortOrder()
{
	return m_sortOrder;
}