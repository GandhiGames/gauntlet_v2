#include "S_Game.h"
#include "StateManager.h"

S_Game::S_Game(StateManager* stateManager) : Scene(stateManager), 
m_level(m_stateManager->m_context), m_pathFinder(m_level)
{
	auto context = m_stateManager->m_context;

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

	m_player->AddComponent<C_Velocity>();
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

	//m_player->AddComponent<C_CollidableTest>();
	//m_player->AddComponent<C_RaycastTest>();

	m_player->AddComponent<C_Camera>();

	Object::Add(m_player);
	//Player setup end.

	for (int i = 0; i < 10; ++i)
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
		collider2->SetTrigger(false);

		follower->AddComponent<C_Velocity>();
		follower->AddComponent<C_Pathfinding>();
		auto followerAnim = follower->AddComponent<C_DirectionalAnimation>();
		followerAnim->SetTextures(textureIDs);

		follower->AddComponent<C_Seperation>();
		follower->AddComponent<C_BehaviorApplier>();

		Object::Add(follower);
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

	Object::UpdateAll(deltaTime);
}

void S_Game::Draw(float deltaTime)
{
	auto window = m_stateManager->m_context->m_window;

	m_level.Draw(*window, deltaTime);

	Object::DrawAll(*window, deltaTime);

	Debug::Draw(*window);
}

void S_Game::LateUpdate(float deltaTime)
{
	Object::LateUpdateAll(deltaTime);

	bool removed = Object::ProcessRemovals();

	if (removed)
	{
		m_collisions.ProcessRemovals();
	}

	// has to be processed before Object::ProcessNewObjects.
	m_collisions.ProcessNewObjects();

	Object::ProcessNewObjects();
}


