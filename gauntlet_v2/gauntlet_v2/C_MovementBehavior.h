#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Util.h"
#include "Component.h"

class C_MovementBehavior : public Component
{
public:
	C_MovementBehavior(Object * owner);
	~C_MovementBehavior();

	int GetWeight() const;

	virtual const sf::Vector2f GetForce() = 0;

protected:
	std::vector<std::shared_ptr<Object>> GetEntitiesInSight(float sightRadius, const std::string& tag) const;

	void SetWeight(int weight);

protected:
	//sf::Vector2f& m_steeringForce;
	int m_weight;
};

