#include "AnimationFactory.h"


std::shared_ptr<std::map<MOVEMENT_DIRECTION, Animation>> AnimationFactory::CreateAnimations(SPRITE_TYPE animationType, int textureID, int startFrame, 
	int endFrame, int animationSpeed, int spriteSize, bool loop)
{
	std::map<MOVEMENT_DIRECTION, Animation> animations;

	sf::Texture& texture = TextureManager::GetTexture(textureID);

	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, Animation(animationType, texture, 0, spriteSize, spriteSize, startFrame, endFrame, animationSpeed, loop)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, Animation(animationType, texture, 1, spriteSize, spriteSize, startFrame, endFrame, animationSpeed, loop)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, Animation(animationType, texture, 2, spriteSize, spriteSize, startFrame, endFrame, animationSpeed, loop)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, Animation(animationType, texture, 3, spriteSize, spriteSize, startFrame, endFrame, animationSpeed, loop)));

	return std::make_shared<std::map<MOVEMENT_DIRECTION, Animation>>(animations);
}
