#pragma once

#include "C_Drawable.h"
#include "C_Updateable.h"
#include "C_Transform.h"

//TODO: create animation component
class C_StaticSprite : public Component, public C_Drawable, public C_Updateable
{
public:
	C_StaticSprite(Object* owner);
	
	void Update(float deltaTime) override;

	/**
	* Draws the object to the screen at its current position.
	* @param window The render window to draw the object to.
	* @param tileDelta The time, in MS, since the last draw call.
	*/
	void Draw(sf::RenderWindow &window, float timeDelta) override;

	/**
	* Creates and sets the object sprite.
	* This function takes the location to a resource, and from that create a texture and sprite.
	* You can optionally specify animation properties. If set the frame dimensions will be calculated automatically.
	* If left blank, the whole resource will be used.
	* @param fileName The path to the resource that you wish to load, relative to the project.
	* @param frames The number of frames in the sprite. Defaults to 1.
	* @param frameSpeed The speed that the animation plays at. Defaults to 1.
	* @return true if the operation succeeded.
	*/
	void SetSprite(sf::Texture& texture);

	void SetSprite(sf::Texture& texture, sf::IntRect rect);

	/**
	* Returns a reference the object's sprite.
	* @return A reference to the object's sprite.
	*/
	const sf::Sprite& GetSprite() const;

	void SetColour(const sf::Color& colour);

	void SetScale(sf::Vector2f& scale);

private:
	/**
	* The object's sprite.
	*/
	sf::Sprite m_sprite;
};

