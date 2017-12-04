#include "AnimationFactory.h"


std::shared_ptr<std::map<MOVEMENT_DIRECTION, Animation>> AnimationFactory::CreateAnimations(int textureID, int startFrame, int endFrame, int animationSpeed, int spriteSize)
{
	std::map<MOVEMENT_DIRECTION, Animation> animations;

	sf::Texture& texture = TextureManager::GetTexture(textureID);

	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, Animation(texture, 0, spriteSize, spriteSize, startFrame, endFrame, animationSpeed)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, Animation(texture, 1, spriteSize, spriteSize, startFrame, endFrame, animationSpeed)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, Animation(texture, 2, spriteSize, spriteSize, startFrame, endFrame, animationSpeed)));
	animations.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, Animation(texture, 3, spriteSize, spriteSize, startFrame, endFrame, animationSpeed)));

	return std::make_shared<std::map<MOVEMENT_DIRECTION, Animation>>(animations);
}
