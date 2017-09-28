#pragma once

#include <SFML\Graphics.hpp>

class Scene 
{
	friend class StateManager;
public:
	Scene(StateManager* stateManager)
		:m_stateManager(stateManager), m_transparent(false),
		m_transcendent(false) {}
	virtual ~Scene() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;

	void SetTransparent(const bool& l_transparent) { m_transparent = l_transparent; }
	bool IsTransparent()const { return m_transparent; }
	void SetTranscendent(const bool& l_transcendence) { m_transcendent = l_transcendence; }
	bool IsTranscendent()const { return m_transcendent; }
	sf::View& GetView() { return m_view; }
	StateManager* GetStateManager() { return m_stateManager; }

protected:
	StateManager* m_stateManager;
	bool m_transparent;
	bool m_transcendent;
	sf::View m_view;
};