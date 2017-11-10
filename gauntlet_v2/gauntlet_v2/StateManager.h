#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <SFML\Graphics.hpp>

#include "Scene.h"
#include "SharedContext.h"
#include "S_Game.h"
#include "S_Intro.h"
#include "S_Paused.h"

enum class StateType { Intro = 1, MainMenu, Game, Paused, GameOver, Credits };

// State container.
using StateContainer = std::vector<std::pair<StateType, Scene*>>;
// Type container.
using TypeContainer = std::vector<StateType>;
// State factory.
using StateFactory = std::unordered_map<StateType, std::function<Scene*(void)>>;

class StateManager 
{
public:
	StateManager(SharedContext* l_shared);
	~StateManager();

	void Update(float deltaTime);
	void Draw(float deltaTime);
	void LateUpdate(float deltaTime);

	void ProcessRequests();

	bool HasState(const StateType& l_type);

	void SwitchTo(const StateType& l_type);
	void Remove(const StateType& l_type);

public:
	SharedContext* m_context;

private:
	void CreateState(const StateType& l_type);
	void RemoveState(const StateType& l_type);

	//TODO: convert to dynamic class instantiation (see Object::AddComponent).
	template<class T>
	void RegisterState(const StateType& l_type) 
	{
		m_stateFactory[l_type] = [this]() -> Scene*
		{
			return new T(this);
		};
	}

private:
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};