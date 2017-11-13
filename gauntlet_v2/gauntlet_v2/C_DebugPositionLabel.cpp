#include "C_DebugPositionLabel.h"
#include "Object.h"


C_DebugPositionLabel::C_DebugPositionLabel(Object* owner) : Component(owner), 
m_offset(0.f, 0.f)
{
	//TODO: create generic resource manager that includes fonts
	m_font.loadFromFile("../resources/fonts/Gisha.ttf");
}

void C_DebugPositionLabel::Draw(sf::RenderWindow &window, float timeDelta)
{
	auto pos = m_owner->m_transform->GetPosition();
	std::string textString = std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y);
	sf::Text text(textString, m_font);
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Red);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(pos + m_offset);

	window.draw(text);
}

void C_DebugPositionLabel::SetOffset(sf::Vector2f offset)
{
	m_offset = offset;
}

