#pragma once

#include <array>
#include <SFML\Graphics.hpp>

#include "Raycast.h"

class Debug
{
public:
	Debug();
	~Debug();

	static void Draw(sf::RenderWindow& window);

	static void DrawLine(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour = sf::Color::White);
	static void DrawRay(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour = sf::Color::White);

	static void Log(const sf::Vector2f& v);
	static void Log(const char* msg);
	static void LogWarning(const char* msg);
	static void LogError(const char* msg);

private:
	static std::vector<std::array<sf::Vertex, 2>> m_lines;
};

