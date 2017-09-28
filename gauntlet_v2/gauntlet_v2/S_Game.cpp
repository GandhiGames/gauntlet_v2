#include "S_Game.h"
#include "StateManager.h"

S_Game::S_Game(StateManager* stateManager) : Scene(stateManager), 
m_level(m_stateManager->m_context), m_pathFinder(m_level)
{
	auto context = m_stateManager->m_context;

	context->m_objects = &m_objects;
	context->m_newObjects = &m_newObjects;
	context->m_level = &m_level;
	context->m_player = &m_player;
	context->m_pathFinder = &m_pathFinder;
}

S_Game::~S_Game()
{
}

void S_Game::OnCreate()
{
	sf::Vector2u size = m_stateManager->m_context->m_window->getSize();
	m_view.setSize((float)size.x, (float)size.y);
	m_view.setCenter(size.x / 2.f, size.y / 2.f);
	m_view.zoom(0.6f);
	m_stateManager->m_context->m_window->setView(m_view);

	m_level.GenerateLevel();

	m_player = std::make_shared<Object>(*m_stateManager->m_context);

	//TODO: can we guarantee that player will stay at index 0.
	// Player setup start.
	m_player->m_transform->SetPosition(m_level.GetSpawnLocation());
	m_player->GetComponent<C_Tag>()->Set(PLAYER_TAG);

	m_player->AddComponent<C_Movement>();
	m_player->AddComponent<C_KeyboardController>();

	std::string className = "archer";
	std::array<int, static_cast<int>(ANIMATION_STATE::COUNT)> textureIDs;
	textureIDs[static_cast<int>(ANIMATION_STATE::WALK_UP)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_walk_up.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::WALK_DOWN)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_walk_down.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::WALK_RIGHT)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_walk_right.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::WALK_LEFT)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_walk_left.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::IDLE_UP)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_idle_up.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::IDLE_DOWN)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_idle_down.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::IDLE_RIGHT)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_idle_right.png");
	textureIDs[static_cast<int>(ANIMATION_STATE::IDLE_LEFT)] = TextureManager::AddTexture("../resources/players/" + className + "/spr_" + className + "_idle_left.png");

	auto obj1Sprite = m_player->AddComponent<C_AnimatedSprite>();
	obj1Sprite->SetSprite(TextureManager::GetTexture(textureIDs[static_cast<int>(ANIMATION_STATE::WALK_UP)]), false, 8, 12);
	obj1Sprite->SetSortOrder(1000);

	auto anim = m_player->AddComponent<C_DirectionalAnimation>();
	anim->SetTextures(textureIDs);

	auto collider1 = m_player->AddComponent<C_BoxCollider>();
	collider1->SetCollidable(sf::FloatRect(obj1Sprite->GetSprite().getGlobalBounds()));
	collider1->SetLayer(CollisionLayer::Player);

	// Set initial sprite.
	obj1Sprite->SetSprite(TextureManager::GetTexture(textureIDs[static_cast<int>(ANIMATION_STATE::WALK_UP)]), false, 8, 12);
	obj1Sprite->SetOrigin(sf::Vector2f(13.f, 18.f));

	m_player->AddComponent<C_CollidableTest>();

	m_player->AddComponent<C_Camera>();

	m_newObjects.push_back(m_player);
	//Player setup end.

	for (int i = 0; i < 250; ++i)
	{
		auto follower = std::make_shared<Object>(*m_stateManager->m_context);

		follower->m_tag->Set(FOLLOWER_TAG);

		auto objSprite = follower->AddComponent<C_AnimatedSprite>();
		objSprite->SetSprite(TextureManager::GetTexture(textureIDs[static_cast<int>(ANIMATION_STATE::WALK_UP)]), false, 8, 12);
		objSprite->SetSortOrder(0);

		follower->m_transform->SetPosition(m_level.GetRandomSpawnLocation());

		auto collider2 = follower->AddComponent<C_BoxCollider>();
		collider2->SetCollidable(sf::FloatRect(objSprite->GetSprite().getGlobalBounds()));
		collider2->SetLayer(CollisionLayer::Followers);
		collider2->SetTrigger(true);

		follower->AddComponent<C_Movement>();
		follower->AddComponent<C_Pathfinding>();
		auto followerAnim = follower->AddComponent<C_DirectionalAnimation>();
		followerAnim->SetTextures(textureIDs);

		m_newObjects.push_back(follower);
	}
}

void S_Game::OnDestroy()
{

}

void S_Game::Activate() {}

void S_Game::Deactivate() {}

void S_Game::Update(float deltaTime)
{
	m_collisions.Resolve();

	for (const auto& obj : m_objects)
	{
		obj->Update(deltaTime);
	}
}

void S_Game::Draw(float deltaTime)
{
	auto window = m_stateManager->m_context->m_window;

	m_level.Draw(*window, deltaTime);

	for (const auto& obj : m_objects)
	{
		obj->Draw(*window, deltaTime);
	}
}

void S_Game::LateUpdate(float deltaTime)
{
	for (const auto& obj : m_objects)
	{
		obj->LateUpdate(deltaTime);
	}

	//Process removals;
	bool objRemoved = false;
	auto objIterator = m_objects.begin();
	while (objIterator != m_objects.end())
	{
		auto obj = **objIterator;

		bool remove = obj.IsQueuedForRemoval();

		if (remove)
		{
			objRemoved = true;
			objIterator = m_objects.erase(objIterator);
		}
		else
		{
			++objIterator;
		}
	}

	if (objRemoved)
	{
		m_collisions.ProcessRemovals();
	}

	// Move newly created objects to global list.
	if (m_newObjects.size() > 0)
	{
		m_collisions.UpdateCollidables(m_newObjects);

		for (auto& obj : m_newObjects)
		{
			m_objects.push_back(obj);
		}
		m_newObjects.clear();

		// Sort them so they are drawn in correct order.
		std::sort(m_objects.begin(), m_objects.end(), by_draw_order());
	}

	m_pathFinder.ResetExecutionCount();
}
