#include "Debug.h"
#include "SharedContext.h"
#include "DungeonGenerator.h"

std::vector<std::array<sf::Vertex, 2>> Debug::m_lines = {};
sf::Font Debug::m_debugFont = {};
std::vector<sf::Text> Debug::m_coordinateTexts = {};
bool Debug::m_drawGridCoordinates = false;

void Debug::Initialise(SharedContext& context)
{
	//TODO: download font and test this.
	//m_debugFont.loadFromFile("arial.ttf");

	auto level = context.m_level;
	auto nodes = level->GetNodes();

	m_coordinateTexts.reserve(nodes.GetWidth() * nodes.GetHeight());

	for (int x = 0; x < nodes.GetWidth(); x++)
	{
		for (int y = 0; y < nodes.GetHeight(); y++)
		{
			auto tile = nodes.GetTile(x, y);
			std::string textString = "test"; // x + ", " + y;
			sf::Text text(textString , m_debugFont);
			text.setCharacterSize(30);
			text.setStyle(sf::Text::Bold);
			text.setFillColor(sf::Color::Red);
			text.setPosition(level->GetTilePosition(x, y));

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

void Debug::Log(const sf::Vector2f& v)
{
	char result[100];
	sprintf_s(result, "\n{%d, %d}", v.x, v.y);
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