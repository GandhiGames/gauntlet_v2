#include "S_Intro.h"
#include "StateManager.h"

S_Intro::S_Intro(StateManager* stateManager) : Scene(stateManager)
{
}


S_Intro::~S_Intro()
{
}

void S_Intro::OnCreate()
{

}

void S_Intro::OnDestroy()
{

}

void S_Intro::Activate()
{

}

void S_Intro::Deactivate()
{

}

void S_Intro::Update(float deltaTime)
{
	if (Input::IsKeyPressed(Input::KEY::KEY_RETURN))
	{
		m_stateManager->SwitchTo(StateType::Game);
	}
}

void S_Intro::Draw(float deltaTime)
{

}

void S_Intro::LateUpdate(float deltaTime)
{

}
