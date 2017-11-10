#include "C_Camera.h"
#include "Object.h"
#include "SharedContext.h"
#include "Level.h"

C_Camera::C_Camera(Object* owner) : Component(owner)
{
}


C_Camera::~C_Camera()
{
}

void C_Camera::LateUpdate(float deltaTime) 
{
	SharedContext& context = m_owner->m_context;
	sf::RenderWindow* window = context.m_window;
	sf::View view = window->getView();
	
	view.setCenter(m_owner->m_transform->GetPosition());

	/*
	sf::Vector2f viewCenter(window->getView().getCenter());
	sf::Vector2f viewSize(window->getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2.f, viewSize.y / 2.f);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	if (viewSpace.left <= 0)
	{
		view.setCenter(viewSpace.width / 2, view.getCenter().y);
	}
	else if (viewSpace.left + viewSpace.width > GRID_WIDTH * TILE_SIZE)
	{
		view.setCenter((GRID_WIDTH * TILE_SIZE) - (viewSpace.width / 2), view.getCenter().y);
	}

	if (viewSpace.top <= 0)
	{
		view.setCenter(view.getCenter().x, viewSpace.height / 2);
	}
	else if (viewSpace.top + viewSpace.height > GRID_HEIGHT * TILE_SIZE)
	{
		view.setCenter(view.getCenter().x, (GRID_HEIGHT * TILE_SIZE) - (viewSpace.height / 2));
	}
	*/

	window->setView(view);
}
