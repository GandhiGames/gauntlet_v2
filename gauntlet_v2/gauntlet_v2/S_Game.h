#pragma once

#include <array>
#include <iterator>
#include <set>

#include "Scene.h"
#include "Object.h"
#include "TextureManager.h"
#include "Level.h"
#include "Util.h"
#include "CollisionResolver.h"
#include "PathFinder.h"
#include "Debug.h"

class S_Game : public Scene
{
public:
	S_Game(StateManager* stateManager);
	~S_Game();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;
	void LateUpdate(float deltaTime) override;

private:
	CollisionResolver m_collisions;
	std::shared_ptr<Object> m_player;
	Level m_level;
	PathFinder m_pathFinder;
};

