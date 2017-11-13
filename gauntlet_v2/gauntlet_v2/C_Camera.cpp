#include "C_Camera.h"
#include "Object.h"
#include "SharedContext.h"
#include "DungeonGenerator.h"

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

	const sf::Vector2f& playerPos = m_owner->m_transform->GetPosition();
	
	view.setCenter(playerPos);

	sf::Vector2f viewCenter(playerPos);
	sf::Vector2f viewSize(view.getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2.f, viewSize.y / 2.f);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	//TODO: Needs to take into account dungeon offset.
	if (viewSpace.left <= 0)
	{
		view.setCenter(viewSpace.width / 2, view.getCenter().y);
	}
	else if (viewSpace.left + viewSpace.width > DUNGEON_WIDTH * DUNGEON_TILE_SIZE)
	{
		view.setCenter((DUNGEON_WIDTH * DUNGEON_TILE_SIZE) - (viewSpace.width / 2), view.getCenter().y);
	}

	if (viewSpace.top <= 0)
	{
		view.setCenter(view.getCenter().x, viewSpace.height / 2);
	}
	else if (viewSpace.top + viewSpace.height > DUNGEON_HEIGHT * DUNGEON_TILE_SIZE)
	{
		view.setCenter(view.getCenter().x, (DUNGEON_HEIGHT * DUNGEON_TILE_SIZE) - (viewSpace.height / 2));
	}

	window->setView(view);
}
