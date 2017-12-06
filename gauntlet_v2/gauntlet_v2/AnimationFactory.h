#pragma once

#include <memory>
#include <map>

#include "Util.h"
#include "Animation.h"
#include "TextureManager.h"

class AnimationFactory
{
public:
	static std::shared_ptr<std::map<MOVEMENT_DIRECTION, Animation>> CreateAnimations(SPRITE_TYPE animationType, int textureID, int startFrame,
		int EndFrame, int animationSpeed, int spriteSize, bool loop);
};

