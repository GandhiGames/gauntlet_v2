#include "Debug.h"

std::vector<std::array<sf::Vertex, 2>> Debug::m_lines = {};

Debug::Debug()
{
}


Debug::~Debug()
{
}

void Debug::Draw(sf::RenderWindow& window)
{
	for (auto& l : m_lines)
	{
		sf::Vertex line[2] = { l.at(0), l.at(1) };
		window.draw(line, 2, sf::Lines);
	}

	m_lines.clear();
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

void Debug::LogWarning(const char* msg)
{
	printf(msg);
}

void Debug::LogError(const char* msg)
{
	printf(msg);
}