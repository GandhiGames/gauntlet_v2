#pragma once

#include <SFML\Graphics.hpp>

class C_Drawable
{
public:
	C_Drawable();
	virtual ~C_Drawable();

	virtual void Draw(sf::RenderWindow &window, float timeDelta) = 0;

	void SetSortOrder(const int& order);
	const int& GetSortOrder();

private:
	int m_sortOrder;
};

