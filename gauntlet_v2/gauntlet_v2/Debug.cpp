#include "Debug.h"
#include "SharedContext.h"
#include "DungeonGenerator.h"

std::vector<std::array<sf::Vertex, 2>> Debug::m_lines = {};
std::vector<sf::RectangleShape> Debug::m_rects = {};
sf::Font Debug::m_debugFont = {};
std::vector<sf::Text> Debug::m_coordinateTexts = {};
bool Debug::m_drawGridCoordinates = false;

void Debug::Initialise(SharedContext& context)
{
	//TODO: create generic resource manager that includes fonts
	m_debugFont.loadFromFile("../resources/fonts/Gisha.ttf");

	auto level = context.m_level;
	auto nodes = level->GetNodes();

	m_coordinateTexts.reserve(nodes.GetWidth() * nodes.GetHeight());

	for (int x = 0; x < nodes.GetWidth(); x++)
	{
		for (int y = 0; y < nodes.GetHeight(); y++)
		{
			auto tile = nodes.GetTile(x, y);
			std::string textString = std::to_string(x) + "," + std::to_string(y);
			sf::Text text(textString , m_debugFont);
			text.setCharacterSize(10);
			//text.setStyle(sf::Text::Bold);
			text.setFillColor(sf::Color::White);
			sf::FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
		
			float tileHalf = DUNGEON_TILE_SIZE * 0.5f;
			text.setPosition(level->GetTilePosition(x, y) +
				sf::Vector2f(tileHalf , tileHalf));

			m_coordinateTexts.push_back(text);
		}
	}
}

void Debug::Draw(sf::RenderWindow& window)
{
	for (auto& l : m_lines)
	{
		sf::Vertex line[2] = { l.at(0), l.at(1) };
		window.draw(line, 2, sf::Lines);
	}
	m_lines.clear();

	for (auto& r : m_rects)
	{
		window.draw(r);
	}
	m_rects.clear();

	if (m_drawGridCoordinates)
	{
		for (auto& t : m_coordinateTexts)
		{
			window.draw(t);
		}
	}
}

void Debug::DrawLine(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour)
{
	m_lines.push_back({ sf::Vertex(from, colour), sf::Vertex(to, colour) });
}

void Debug::DrawRay(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour)
{
	std::vector<sf::Vector2f> lines = Raycast::BresenhamLine(from, to);

	for (int i = 0; i < lines.size(); i++)
	{
		if (i < lines.size() - 1)
		{
			m_lines.push_back({ sf::Vertex(lines[i], colour), sf::Vertex(lines[i + 1], colour) });
		}
	}
}

void Debug::DrawRect(const sf::Vector2f& pos, const sf::Vector2f size, sf::Color colour)
{
	sf::RectangleShape shape(size);
	shape.setPosition(pos);
	shape.setFillColor(colour);
	m_rects.push_back(shape);
}

void Debug::Log(const sf::Vector2f& v)
{
	char result[100];
	sprintf_s(result, "\n{%f, %f}", v.x, v.y);
	printf(result);
}

void Debug::Log(const char* msg)
{
	printf(msg);
}

//TODO: differentiate between log, warning, and error
void Debug::LogWarning(const char* msg)
{
	printf(msg);
}

void Debug::LogError(const char* msg)
{
	printf(msg);
}

void Debug::ToggleGridCoordinates(bool draw)
{
	m_drawGridCoordinates = draw;
}