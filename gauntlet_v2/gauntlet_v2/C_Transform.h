#pragma once

#include <memory>

#include <SFML\Graphics.hpp>

#include "Component.h"

//TODO: have method of marking transform static so that sprite (and any other components that rely on it) do
// not keep polling getposition.

// Automatically added to all new objects.
class C_Transform : public Component
{
public:
	C_Transform(Object* owner);
	~C_Transform();

	/**
	* Sets the position of the object on screen. This is relative to the top-left of the game window.
	* @param position The new position of the player.
	*/
	void SetPosition(sf::Vector2f position);

	//TODO: should we clamp values so they do not enter tiles.
	void SetPosition(float x, float y);

	void AddPosition(sf::Vector2f position);


	void SetX(float x);

	void SetY(float y);

	/**
	* Returns the position of the object. This is relative to the top-left of the game window.
	* @return The position of the object
	*/
	const sf::Vector2f GetPosition() const;

	void SetParent(std::shared_ptr<C_Transform> parent);
	const std::shared_ptr<C_Transform> GetParent() const;

	void AddChild(std::shared_ptr<C_Transform> child);
	void RemoveChild(std::shared_ptr<C_Transform> child);

	const std::vector<std::shared_ptr<C_Transform>>& GetChildren() const;


private:
	/**
	* The position of the object in the game window.
	*/
	sf::Vector2f m_position;
	std::shared_ptr<C_Transform> m_parent;
	std::vector<std::shared_ptr<C_Transform>> m_children;
};