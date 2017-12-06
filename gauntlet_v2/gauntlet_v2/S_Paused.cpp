#include "S_Paused.h"
#include "StateManager.h"
#include "Object.h"

S_Paused::S_Paused(StateManager* stateManager) : Scene(stateManager)
{
	SetTransparent(true);
}


S_Paused::~S_Paused()
{
}

void S_Paused::OnCreate()
{

}

void S_Paused::OnDestroy()
{

}

void S_Paused::Activate()
{

}

void S_Paused::Deactivate()
{

}

void S_Paused::Update(float deltaTime)
{
	if (Input::IsKeyPressed(Input::KEY::KEY_ESC))
	{
		m_stateManager->SwitchTo(StateType::Game);
	}
	else if (Input::IsKeyPressed(Input::KEY::KEY_RETURN))
	{
		m_stateManager->m_context->m_window->close();
	}
}

void S_Paused::Draw(float deltaTime)
{
	
}

void S_Paused::LateUpdate(float deltaTime)
{
	//m_stateManager->m_context->m_player->GetComponent<C_Camera>()->LateUpdate(deltaTime);
}

