#pragma once

#include "Scene.h"
#include "Input.h"

class S_Intro : public Scene
{
public:
	S_Intro(StateManager* stateManager);
	~S_Intro();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
};

