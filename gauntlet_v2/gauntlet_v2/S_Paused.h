#pragma once

#include "Scene.h"
#include "Input.h"

class S_Paused : public Scene
{
public:
	S_Paused(StateManager* stateManager);
	~S_Paused();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
};

