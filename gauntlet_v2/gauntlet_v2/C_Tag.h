#pragma once

#include <string>

#include "Component.h"

class C_Tag : public Component
{
public:
	C_Tag(Object* owner);

	const std::string& Get() const;
	void Set(const std::string name);

private:
	std::string m_tag;
};

