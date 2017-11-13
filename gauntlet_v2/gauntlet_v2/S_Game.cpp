#include "S_Game.h"
#include "StateManager.h"

S_Game::S_Game(StateManager* stateManager) : Scene(stateManager), 
m_dungeon(*m_stateManager->m_context)
//m_level(m_stateManager->m_context), m_pathFinder(m_level)
{

}

S_Game::~S_Game()
{
}

void S_Game::OnCreate()
{
	sf::Vector2u size = m_stateManager->m_context->m_window->getSize();
	m_view.setSize((float)size.x, (float)size.y);
	m_view.setCenter(size.x / 2.f, size.y / 2.f);
	//TODO: move to camera component.
	m_view.zoom(0.8f);
	m_stateManager->m_context->m_window->setView(m_view);

	//m_level.GenerateLevel();
	m_dungeon.Generate();

	
	/*******************
	 Player setup start.
	********************/

	m_player = std::make_shared<Object>(*m_stateManager->m_context);

	m_player->m_transform->SetPosition(m_dungeon.GetRandomFloorLocation());
	m_player->GetComponent<C_Tag>()->Set(PLAYER_TAG);

	m_player->AddComponent<C_Velocity>();
	
	auto controller = m_player->AddComponent<C_KeyboardController>();
	controller->SetMovementSpeed(80);

	auto playerSprite = m_player->AddComponent<C_AnimatedSprite>();
	int walkTextureID = TextureManager::AddTexture("../resources/characters/character_male_walk.png");
	sf::Texture& walkTexture = TextureManager::GetTexture(walkTextureID);

	Animation walkUp(walkTexture, 0, 64, 64, 1, 9, 10);
	Animation walkLeft(walkTexture, 1, 64, 64, 1, 9, 10);
	Animation walkDown(walkTexture, 2, 64, 64, 1, 9, 10);
	Animation walkRight(walkTexture, 3, 64, 64, 1, 9, 10);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_UP, walkUp);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_LEFT, walkLeft);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_DOWN, walkDown);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_RIGHT, walkRight);

	Animation idleUp(walkTexture, 0, 64, 64, 0, 0, 0);
	Animation idleLeft(walkTexture, 1, 64, 64, 0, 0, 0);
	Animation idleDown(walkTexture, 2, 64, 64, 0, 0, 0);
	Animation idleRight(walkTexture, 3, 64, 64, 0, 0, 0);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_UP, idleUp);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_LEFT, idleLeft);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_DOWN, idleDown);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_RIGHT, idleRight);

	playerSprite->SetSortOrder(1000);

	m_player->AddComponent<C_DirectionalAnimation>();

	auto collider1 = m_player->AddComponent<C_BoxCollider>();
	collider1->SetCollidable(sf::FloatRect(playerSprite->GetSprite()->getGlobalBounds()));
	collider1->SetLayer(CollisionLayer::Player);

	//m_player->AddComponent<C_CollidableTest>();
	//m_player->AddComponent<C_RaycastTest>();

	m_player->AddComponent<C_Camera>();

	Object::Add(m_player);

	// Player Hair
	/*
	auto hairObj = std::make_shared<Object>(*m_stateManager->m_context);
	hairObj->m_transform->SetParent(m_player->m_transform);
	auto hairSprite = hairObj->AddComponent<C_AnimatedSprite>();
	hairSprite->SetSortOrder(1001);

	int hairTextureID = TextureManager::AddTexture("../resources/characters/character_hair_blonde_walk.png");
	sf::Texture& hairTexture = TextureManager::GetTexture(walkTextureID);

	Animation hairUp(hairTexture, 0, 64, 64, 0, 8, 10);
	Animation hairLeft(hairTexture, 1, 64, 64, 0, 8, 10);
	Animation hairDown(hairTexture, 2, 64, 64, 0, 8, 10);
	Animation hairRight(hairTexture, 3, 64, 64, 0, 8, 10);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_UP, walkUp);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_LEFT, walkLeft);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_DOWN, walkDown);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_RIGHT, walkRight);
	*/

	/*
	auto shadow = std::make_shared<Object>(*m_stateManager->m_context);
	auto shadowSprite = shadow->AddComponent<C_StaticSprite>();
	auto fileName = "../resources/tilesets/dungeon_tileset.png";
	int tilesetID = TextureManager::AddTexture(fileName);
	shadowSprite->SetSprite(TextureManager::GetTexture(tilesetID), sf::IntRect(16, 512, 16, 16));
	shadowSprite->SetSortOrder(999);
	shadowSprite->SetScale(sf::Vector2f(.5f, .5f));
	shadow->m_transform->SetParent(m_player->m_transform);
	shadow->m_transform->SetPosition(sf::Vector2f(0.f, 7.f));
	Object::Add(shadow);
	*/

	auto label = m_player->AddComponent<C_DebugPositionLabel>();
	label->SetOffset(sf::Vector2f(0.f, -35.f));
	label->SetSortOrder(1000);
	/*******************
	 Player setup end.
	********************/

	/*
	for (int i = 0; i < 10; ++i)
	{
		auto follower = std::make_shared<Object>(*m_stateManager->m_context);

		follower->m_tag->Set(FOLLOWER_TAG);

		auto objSprite = follower->AddComponent<C_AnimatedSprite>();
		objSprite->SetSprite(TextureManager::GetTexture(textureID), false, 0, 32, 32, 8, 10);
		objSprite->SetSortOrder(0);
		objSprite->SetScale(sf::Vector2f(.5f, .5f));

		follower->m_transform->SetPosition(m_level.GetRandomSpawnLocation());

		auto collider2 = follower->AddComponent<C_BoxCollider>();
		collider2->SetCollidable(sf::FloatRect(objSprite->GetSprite().getGlobalBounds()));
		collider2->SetLayer(CollisionLayer::Followers);
		collider2->SetTrigger(true);

		follower->AddComponent<C_Velocity>();
		follower->AddComponent<C_Pathfinding>();
		follower->AddComponent<C_DirectionalAnimation>();

		follower->AddComponent<C_Seperation>();
		follower->AddComponent<C_BehaviorApplier>();

		Object::Add(follower);
	}
	*/


	auto context = m_stateManager->m_context;
	context->m_level = &m_dungeon;
	context->m_player = m_player;
	context->m_pathFinder = &m_pathFinder;

	Debug::Initialise(*context);

	Debug::ToggleGridCoordinates(true);
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

	if (Input::IsKeyPressed(Input::KEY::KEY_ESC))
	{
		m_stateManager->m_context->m_window->close();
	}
}

void S_Game::Draw(float deltaTime)
{
	auto window = m_stateManager->m_context->m_window;

	m_dungeon.Draw(*window, deltaTime);

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


