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
	struct by_draw_order
	{
		bool operator()(std::shared_ptr<Object> &a, std::shared_ptr<Object> &b)
		{
			auto sprite1 = a->GetComponent<C_Drawable>();
			auto sprite2 = b->GetComponent<C_Drawable>();

			if (!sprite1 && !sprite2)
			{
				return false;
			}

			if (sprite1 && !sprite2)
			{
				return false;
			}

			if (!sprite1 && sprite2)
			{
				return true;
			}

			if (sprite1->GetSortOrder() == sprite2->GetSortOrder())
			{
				return false;
			}
			else if (sprite1->GetSortOrder() < sprite2->GetSortOrder())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};


private:
	// Do not add directly to m_objects. Add to m_newObjects instead.
	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Object>> m_newObjects;

	CollisionResolver m_collisions;

	std::shared_ptr<Object> m_player;

	Level m_level;

	PathFinder m_pathFinder;
};

