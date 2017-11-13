#pragma once

#include "Component.h"
#include "C_Drawable.h"

class C_DebugPositionLabel : public Component, public C_Drawable
{
public:
	C_DebugPositionLabel(Object* owner);

	void Draw(sf::RenderWindow &window, float timeDelta);
	void SetOffset(sf::Vector2f offset);

private:
	sf::Font m_font;
	sf::Vector2f m_offset;
};

