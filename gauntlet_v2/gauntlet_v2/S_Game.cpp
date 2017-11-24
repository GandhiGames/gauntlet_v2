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
	m_view.zoom(.8f);
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
	controller->SetMovementSpeed(120);

	auto playerSprite = m_player->AddComponent<C_AnimatedSprite>();
	
	//TODO: move this a text/data file.
	const int animationSpeed = 14;
	const int startFrame = 1;
	const int endFrame = 9;
	/* Body Animatons */
	int walkTextureID = TextureManager::AddTexture("../resources/characters/character_male_walk.png");
	sf::Texture& walkTexture = TextureManager::GetTexture(walkTextureID);

	Animation walkUp(walkTexture, 0, 64, 64, startFrame, endFrame, animationSpeed);
	Animation walkLeft(walkTexture, 1, 64, 64, startFrame, endFrame, animationSpeed);
	Animation walkDown(walkTexture, 2, 64, 64, startFrame, endFrame, animationSpeed);
	Animation walkRight(walkTexture, 3, 64, 64, startFrame, endFrame, animationSpeed);

	Animation idleUp(walkTexture, 0, 64, 64, 0, 0, 0);
	Animation idleLeft(walkTexture, 1, 64, 64, 0, 0, 0);
	Animation idleDown(walkTexture, 2, 64, 64, 0, 0, 0);
	Animation idleRight(walkTexture, 3, 64, 64, 0, 0, 0);

	/* Hair Animations */
	int hairTextureID = TextureManager::AddTexture("../resources/characters/character_hair_blonde_walk.png");
	sf::Texture& hairTexture = TextureManager::GetTexture(hairTextureID);

	Animation hairWalkUp(hairTexture, 0, 64, 64, startFrame, endFrame, animationSpeed);
	Animation hairWalkLeft(hairTexture, 1, 64, 64, startFrame, endFrame, animationSpeed);
	Animation hairWalkDown(hairTexture, 2, 64, 64, startFrame, endFrame, animationSpeed);
	Animation hairWalkRight(hairTexture, 3, 64, 64, startFrame, endFrame, animationSpeed);

	Animation hairIdleUp(hairTexture, 0, 64, 64, 0, 0, 0);
	Animation hairIdleLeft(hairTexture, 1, 64, 64, 0, 0, 0);
	Animation hairIdleDown(hairTexture, 2, 64, 64, 0, 0, 0);
	Animation hairIdleRight(hairTexture, 3, 64, 64, 0, 0, 0);

	/* Pants Animations */
	int pantsTextureID = TextureManager::AddTexture("../resources/characters/character_pants_green_walk.png");
	sf::Texture& pantsTexture = TextureManager::GetTexture(pantsTextureID);

	Animation pantsWalkUp(pantsTexture, 0, 64, 64, startFrame, endFrame, animationSpeed);
	Animation pantsWalkLeft(pantsTexture, 1, 64, 64, startFrame, endFrame, animationSpeed);
	Animation pantsWalkDown(pantsTexture, 2, 64, 64, startFrame, endFrame, animationSpeed);
	Animation pantsWalkRight(pantsTexture, 3, 64, 64, startFrame, endFrame, animationSpeed);

	Animation pantsIdleUp(pantsTexture, 0, 64, 64, 0, 0, 0);
	Animation pantsIdleLeft(pantsTexture, 1, 64, 64, 0, 0, 0);
	Animation pantsIdleDown(pantsTexture, 2, 64, 64, 0, 0, 0);
	Animation pantsIdleRight(pantsTexture, 3, 64, 64, 0, 0, 0);

	/* Shirt Animations */
	int shirtTextureID = TextureManager::AddTexture("../resources/characters/charactera_leather_armor_shirt_white_walk.png");
	sf::Texture& shirtsTexture = TextureManager::GetTexture(shirtTextureID);

	Animation shirtWalkUp(shirtsTexture, 0, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shirtWalkLeft(shirtsTexture, 1, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shirtWalkDown(shirtsTexture, 2, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shirtWalkRight(shirtsTexture, 3, 64, 64, startFrame, endFrame, animationSpeed);

	Animation shirtIdleUp(shirtsTexture, 0, 64, 64, 0, 0, 0);
	Animation shirtIdleLeft(shirtsTexture, 1, 64, 64, 0, 0, 0);
	Animation shirtIdleDown(shirtsTexture, 2, 64, 64, 0, 0, 0);
	Animation shirtIdleRight(shirtsTexture, 3, 64, 64, 0, 0, 0);

	/* Shoes Animation */
	int shoesTextureID = TextureManager::AddTexture("../resources/characters/character_shoes_brown_walk.png");
	sf::Texture& shoesTexture = TextureManager::GetTexture(shoesTextureID);

	Animation shoesWalkUp(shoesTexture, 0, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shoesWalkLeft(shoesTexture, 1, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shoesWalkDown(shoesTexture, 2, 64, 64, startFrame, endFrame, animationSpeed);
	Animation shoesWalkRight(shoesTexture, 3, 64, 64, startFrame, endFrame, animationSpeed);

	Animation shoesIdleUp(shoesTexture, 0, 64, 64, 0, 0, 0);
	Animation shoesIdleLeft(shoesTexture, 1, 64, 64, 0, 0, 0);
	Animation shoesIdleDown(shoesTexture, 2, 64, 64, 0, 0, 0);
	Animation shoesIdleRight(shoesTexture, 3, 64, 64, 0, 0, 0);

	// Group Animations
	AnimationGroup playerWalkUp;
	playerWalkUp.AddAnimation(walkUp);
	playerWalkUp.AddAnimation(hairWalkUp);
	playerWalkUp.AddAnimation(pantsWalkUp);
	playerWalkUp.AddAnimation(shirtWalkUp);
	playerWalkUp.AddAnimation(shoesWalkUp);
	AnimationGroup playerWalkLeft;
	playerWalkLeft.AddAnimation(walkLeft);
	playerWalkLeft.AddAnimation(hairWalkLeft);
	playerWalkLeft.AddAnimation(pantsWalkLeft);
	playerWalkLeft.AddAnimation(shirtWalkLeft);
	playerWalkLeft.AddAnimation(shoesWalkLeft);
	AnimationGroup playerWalkRight;
	playerWalkRight.AddAnimation(walkRight);
	playerWalkRight.AddAnimation(hairWalkRight);
	playerWalkRight.AddAnimation(pantsWalkRight);
	playerWalkRight.AddAnimation(shirtWalkRight);
	playerWalkRight.AddAnimation(shoesWalkRight);
	AnimationGroup playerWalkDown;
	playerWalkDown.AddAnimation(walkDown);
	playerWalkDown.AddAnimation(hairWalkDown);
	playerWalkDown.AddAnimation(pantsWalkDown);
	playerWalkDown.AddAnimation(shirtWalkDown);
	playerWalkDown.AddAnimation(shoesWalkDown);

	AnimationGroup playerIdleUp;
	playerIdleUp.AddAnimation(idleUp);
	playerIdleUp.AddAnimation(hairIdleUp);
	playerIdleUp.AddAnimation(pantsIdleUp);
	playerIdleUp.AddAnimation(shirtIdleUp);
	playerIdleUp.AddAnimation(shoesIdleUp);
	AnimationGroup playerIdleLeft;
	playerIdleLeft.AddAnimation(idleLeft);
	playerIdleLeft.AddAnimation(hairIdleLeft);
	playerIdleLeft.AddAnimation(pantsIdleLeft);
	playerIdleLeft.AddAnimation(shirtIdleLeft);
	playerIdleLeft.AddAnimation(shoesIdleLeft);
	AnimationGroup playerIdleRight;
	playerIdleRight.AddAnimation(idleRight);
	playerIdleRight.AddAnimation(hairIdleRight);
	playerIdleRight.AddAnimation(pantsIdleRight);
	playerIdleRight.AddAnimation(shirtIdleRight);
	playerIdleRight.AddAnimation(shoesIdleRight);
	AnimationGroup playerIdleDown;
	playerIdleDown.AddAnimation(idleDown);
	playerIdleDown.AddAnimation(hairIdleDown);
	playerIdleDown.AddAnimation(pantsIdleDown);
	playerIdleDown.AddAnimation(shirtIdleDown);
	playerIdleDown.AddAnimation(shoesIdleDown);

	playerSprite->AddAnimation(ANIMATION_STATE::WALK_UP, playerWalkUp);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_LEFT, playerWalkLeft);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_DOWN, playerWalkDown);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_RIGHT, playerWalkRight);


	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_UP, playerIdleUp);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_LEFT, playerIdleLeft);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_DOWN, playerIdleDown);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_RIGHT, playerIdleRight);

	playerSprite->SetSortOrder(1000);

	m_player->AddComponent<C_DirectionalAnimation>();

	auto collider1 = m_player->AddComponent<C_BoxCollider>();
	collider1->SetCollidable(sf::FloatRect(walkDown.m_sprite.getGlobalBounds()));
	collider1->SetLayer(CollisionLayer::Player);

	//m_player->AddComponent<C_CollidableTest>();
	//m_player->AddComponent<C_RaycastTest>();

	m_player->AddComponent<C_Camera>();

	Object::Add(m_player);

	auto label = m_player->AddComponent<C_DebugPositionLabel>();
	label->SetOffset(sf::Vector2f(0.f, 0.f));
	label->SetSortOrder(1010);

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


