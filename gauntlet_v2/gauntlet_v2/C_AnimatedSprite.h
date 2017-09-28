#pragma once

#include <assert.h>

#include "C_Drawable.h"
#include "C_Updateable.h"
#include "C_Transform.h"

class C_AnimatedSprite : public Component, public C_Drawable, public C_Updateable
{
	friend class Object;

public:
	C_AnimatedSprite(Object* owner);
	~C_AnimatedSprite();

	void Update(float deltaTime) override;

	virtual void Draw(sf::RenderWindow &window, float timeDelta) override;

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
	bool SetSprite(sf::Texture& texture, bool isSmooth, int frames = 1, int frameSpeed = 0);

	const sf::Sprite& GetSprite();

	void SetOrigin(const sf::Vector2f origin);

	void SetTexture(const sf::Texture& texture);

	void NextFrame();

	/**
	* Get the number of frames the object has.
	* @return The number of frames the object has.
	*/
	int GetFrameCount() const;

	void SetAnimated(bool animated);

	bool IsAnimated();

private:
	sf::Sprite m_sprite;

	bool m_animated;

	/**
	* The animation speed of the image if applicable.
	* Value is frames per second.
	*/
	int m_animationSpeed;

	/**
	* The total number of frames the sprite has.
	*/
	int m_frameCount;

	/**
	* The current frame of the sprite.
	*/
	int m_currentFrame;

	/**
	* The width of each frame of the animated sprite if applicable.
	*/
	int m_frameWidth;

	/**
	* The height of each frame of the animated sprite if applicable.
	*/
	int m_frameHeight;

	/**
	* An aggregate of the time passed between draw calls.
	*/
	float m_timeDelta;
};