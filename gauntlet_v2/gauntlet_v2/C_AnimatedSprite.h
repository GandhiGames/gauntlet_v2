#pragma once

#include "C_Drawable.h"
#include "C_Updateable.h"
#include "C_Transform.h"
#include "AnimationGroup.h"
#include "Util.h"

class C_AnimatedSprite : public Component, public C_Drawable, public C_Updateable
{
	friend class Object;

public:
	C_AnimatedSprite(Object* owner);
	~C_AnimatedSprite();

	void LateUpdate(float deltaTime) override;

	virtual void Draw(sf::RenderWindow &window, float timeDelta) override;

	//TODO: look into optomising pass by-reference? convert to data only struct?
	void AddAnimation(ANIMATION_STATE state, AnimationGroup& animationGroup);
	void SetCurrentAnimation(ANIMATION_STATE state);

	//const sf::Sprite* GetSprite();

	void SetAnimated(bool animated);

	bool IsAnimated();
private:
	std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> m_animations;
	
	std::shared_ptr<AnimationGroup> m_curAnimation;

	bool m_animated;
};