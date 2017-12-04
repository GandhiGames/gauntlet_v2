#include "C_Tag.h"


C_Tag::C_Tag(Object* owner) : Component(owner)
{
}

const std::string& C_Tag::Get() const
{
	return m_tag;
}

void C_Tag::Set(const std::string tag)
{
	m_tag = tag;
}