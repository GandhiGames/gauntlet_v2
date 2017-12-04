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
	const int spriteSize = 64;
	
	//TODO: Need to be able to easily swap out animations e.g. you want to be able to change armour/sword parts easily.
	std::map<MOVEMENT_DIRECTION, AnimationGroup> walkAnimationGroups;
	walkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, AnimationGroup()));
	walkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, AnimationGroup()));
	walkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, AnimationGroup()));
	walkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, AnimationGroup()));

	std::map<MOVEMENT_DIRECTION, AnimationGroup> idleAnimationGroups;
	idleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, AnimationGroup()));
	idleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, AnimationGroup()));
	idleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, AnimationGroup()));
	idleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, AnimationGroup()));

	std::map<MOVEMENT_DIRECTION, AnimationGroup> meleeAttackAnimationGroups;
	meleeAttackAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, AnimationGroup()));
	meleeAttackAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, AnimationGroup()));
	meleeAttackAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, AnimationGroup()));
	meleeAttackAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, AnimationGroup()));

	/* Body Animatons */
	int walkTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/walk.png");
	auto bodyWalkAnimations = AnimationFactory::CreateAnimations(walkTextureID, startFrame, endFrame, animationSpeed, spriteSize);
	for (auto a : *bodyWalkAnimations)
	{
		walkAnimationGroups[a.first].AddAnimation(a.second);
	}
	
	auto bodyIdleAnimations = AnimationFactory::CreateAnimations(walkTextureID, 0, 0, 0, spriteSize);
	for (auto a : *bodyIdleAnimations)
	{
		idleAnimationGroups[a.first].AddAnimation(a.second);
	}

	/* Hair Animations */
	int hairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/walk.png");
	auto hairWalkAnimations = AnimationFactory::CreateAnimations(hairTextureID, startFrame, endFrame, animationSpeed, spriteSize);
	for (auto a : *hairWalkAnimations)
	{
		walkAnimationGroups[a.first].AddAnimation(a.second);
	}

	auto hairIdleAnimations = AnimationFactory::CreateAnimations(hairTextureID, 0, 0, 0, spriteSize);
	for (auto a : *hairIdleAnimations)
	{
		idleAnimationGroups[a.first].AddAnimation(a.second);
	}

	/* Pants Animations */
	int pantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/walk.png");
	auto pantsWalkAnimations = AnimationFactory::CreateAnimations(pantsTextureID, startFrame, endFrame, animationSpeed, spriteSize);
	for (auto a : *pantsWalkAnimations)
	{
		walkAnimationGroups[a.first].AddAnimation(a.second);
	}

	auto pantsIdleAnimations = AnimationFactory::CreateAnimations(pantsTextureID, 0, 0, 0, spriteSize);
	for (auto a : *pantsIdleAnimations)
	{
		idleAnimationGroups[a.first].AddAnimation(a.second);
	}


	/* Shirt Animations */
	int shirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/walk.png");
	
	auto shirtWalkAnimations = AnimationFactory::CreateAnimations(shirtTextureID, startFrame, endFrame, animationSpeed, spriteSize);
	for (auto a : *shirtWalkAnimations)
	{
		walkAnimationGroups[a.first].AddAnimation(a.second);
	}

	auto shirtIdleAnimations = AnimationFactory::CreateAnimations(shirtTextureID, 0, 0, 0, spriteSize);
	for (auto a : *shirtIdleAnimations)
	{
		idleAnimationGroups[a.first].AddAnimation(a.second);
	}

	/* Shoes Animation */
	int shoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/walk.png");
	auto shoesWalkAnimations = AnimationFactory::CreateAnimations(shoesTextureID, startFrame, endFrame, animationSpeed, spriteSize);
	for (auto a : *shoesWalkAnimations)
	{
		walkAnimationGroups[a.first].AddAnimation(a.second);
	}

	auto shoesIdleAnimations = AnimationFactory::CreateAnimations(shoesTextureID, 0, 0, 0, spriteSize);
	for (auto a : *shoesIdleAnimations)
	{
		idleAnimationGroups[a.first].AddAnimation(a.second);
	}

	/* Sword Animations */
	int swingBodyTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/swing.png");
	auto swingBodyAttackAnimations = AnimationFactory::CreateAnimations(swingBodyTextureID, 0, 5, animationSpeed, spriteSize);
	for (auto a : *swingBodyAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}

	int swordTextureID = TextureManager::AddTexture("../resources/weapons/sword.png");
	auto swordAttackAnimations = AnimationFactory::CreateAnimations(swordTextureID, 0, 5, animationSpeed, 128);
	for (auto a : *swordAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}

	int swingShoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/swing.png");
	auto swingShoesAttackAnimations = AnimationFactory::CreateAnimations(swingShoesTextureID, 0, 5, animationSpeed, spriteSize);
	for (auto a : *swingShoesAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}

	int swingHairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/swing.png");
	auto swingHairAttackAnimations = AnimationFactory::CreateAnimations(swingHairTextureID, 0, 5, animationSpeed, spriteSize);
	for (auto a : *swingHairAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}

	int swingPantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/swing.png");
	auto swingPantsAttackAnimations = AnimationFactory::CreateAnimations(swingPantsTextureID, 0, 5, animationSpeed, spriteSize);
	for (auto a : *swingPantsAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}

	int swingshirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/swing.png");
	auto swingShirtAttackAnimations = AnimationFactory::CreateAnimations(swingshirtTextureID, 0, 5, animationSpeed, spriteSize);
	for (auto a : *swingShirtAttackAnimations)
	{
		meleeAttackAnimationGroups[a.first].AddAnimation(a.second);
	}



	playerSprite->AddAnimation(ANIMATION_STATE::WALK_UP, walkAnimationGroups[MOVEMENT_DIRECTION::UP]);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_LEFT, walkAnimationGroups[MOVEMENT_DIRECTION::LEFT]);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_DOWN, walkAnimationGroups[MOVEMENT_DIRECTION::DOWN]);
	playerSprite->AddAnimation(ANIMATION_STATE::WALK_RIGHT, walkAnimationGroups[MOVEMENT_DIRECTION::RIGHT]);

	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_UP, idleAnimationGroups[MOVEMENT_DIRECTION::UP]);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_LEFT, idleAnimationGroups[MOVEMENT_DIRECTION::LEFT]);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_DOWN, idleAnimationGroups[MOVEMENT_DIRECTION::DOWN]);
	playerSprite->AddAnimation(ANIMATION_STATE::IDLE_RIGHT, idleAnimationGroups[MOVEMENT_DIRECTION::RIGHT]);

	playerSprite->AddAnimation(ANIMATION_STATE::SWING_UP, meleeAttackAnimationGroups[MOVEMENT_DIRECTION::UP]);
	playerSprite->AddAnimation(ANIMATION_STATE::SWING_LEFT, meleeAttackAnimationGroups[MOVEMENT_DIRECTION::LEFT]);
	playerSprite->AddAnimation(ANIMATION_STATE::SWING_DOWN, meleeAttackAnimationGroups[MOVEMENT_DIRECTION::DOWN]);
	playerSprite->AddAnimation(ANIMATION_STATE::SWING_RIGHT, meleeAttackAnimationGroups[MOVEMENT_DIRECTION::RIGHT]);

	playerSprite->SetSortOrder(1000);

	m_player->AddComponent<C_DirectionalAnimation>();

	auto collider1 = m_player->AddComponent<C_BoxCollider>();
	collider1->SetCollidable(sf::FloatRect(bodyWalkAnimations->at(MOVEMENT_DIRECTION::DOWN).m_sprite.getGlobalBounds()));
	collider1->SetLayer(CollisionLayer::Player);

	//m_player->AddComponent<C_CollidableTest>();
	m_player->AddComponent<C_RaycastTest>();

	m_player->AddComponent<C_Camera>();
	m_player->AddComponent<C_MeleeAttack>();
	m_player->AddComponent<C_Direction>();

	Object::Add(m_player);

	auto label = m_player->AddComponent<C_DebugPositionLabel>();
	label->SetOffset(sf::Vector2f(0.f, 0.f));
	label->SetSortOrder(1010);

	/*******************
	 Player setup end.
	********************/


	for (int i = 0; i < 10; ++i)
	{
		auto follower = std::make_shared<Object>(*m_stateManager->m_context);

		follower->m_tag->Set(FOLLOWER_TAG);

		auto objSprite = follower->AddComponent<C_AnimatedSprite>();

		std::map<MOVEMENT_DIRECTION, AnimationGroup> objWalkAnimationGroups;
		objWalkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, AnimationGroup()));
		objWalkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, AnimationGroup()));
		objWalkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, AnimationGroup()));
		objWalkAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, AnimationGroup()));

		std::map<MOVEMENT_DIRECTION, AnimationGroup> objIdleAnimationGroups;
		objIdleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::UP, AnimationGroup()));
		objIdleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::DOWN, AnimationGroup()));
		objIdleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::LEFT, AnimationGroup()));
		objIdleAnimationGroups.emplace(std::make_pair(MOVEMENT_DIRECTION::RIGHT, AnimationGroup()));

		/* Body Animatons */
		int walkTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/walk.png");
		auto bodyWalkAnimations = AnimationFactory::CreateAnimations(walkTextureID, startFrame, endFrame, animationSpeed, spriteSize);
		for (auto a : *bodyWalkAnimations)
		{
			objWalkAnimationGroups[a.first].AddAnimation(a.second);
		}
		auto bodyIdleAnimations = AnimationFactory::CreateAnimations(walkTextureID, 0, 0, 0, spriteSize);
		for (auto a : *bodyIdleAnimations)
		{
			objIdleAnimationGroups[a.first].AddAnimation(a.second);
		}

		/* Hair Animations */
		int hairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/walk.png");
		auto hairWalkAnimations = AnimationFactory::CreateAnimations(hairTextureID, startFrame, endFrame, animationSpeed, spriteSize);
		for (auto a : *hairWalkAnimations)
		{
			objWalkAnimationGroups[a.first].AddAnimation(a.second);
		}
		auto hairIdleAnimations = AnimationFactory::CreateAnimations(hairTextureID, 0, 0, 0, spriteSize);
		for (auto a : *hairIdleAnimations)
		{
			objIdleAnimationGroups[a.first].AddAnimation(a.second);
		}

		/* Pants Animations */
		int pantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/walk.png");
		auto pantsWalkAnimations = AnimationFactory::CreateAnimations(pantsTextureID, startFrame, endFrame, animationSpeed, spriteSize);
		for (auto a : *pantsWalkAnimations)
		{
			objWalkAnimationGroups[a.first].AddAnimation(a.second);
		}
		auto pantsIdleAnimations = AnimationFactory::CreateAnimations(pantsTextureID, 0, 0, 0, spriteSize);
		for (auto a : *pantsIdleAnimations)
		{
			objIdleAnimationGroups[a.first].AddAnimation(a.second);
		}


		/* Shirt Animations */
		int shirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/walk.png");
		auto shirtWalkAnimations = AnimationFactory::CreateAnimations(shirtTextureID, startFrame, endFrame, animationSpeed, spriteSize);
		for (auto a : *shirtWalkAnimations)
		{
			objWalkAnimationGroups[a.first].AddAnimation(a.second);
		}
		auto shirtIdleAnimations = AnimationFactory::CreateAnimations(shirtTextureID, 0, 0, 0, spriteSize);
		for (auto a : *shirtIdleAnimations)
		{
			objIdleAnimationGroups[a.first].AddAnimation(a.second);
		}

		/* Shoes Animation */
		int shoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/walk.png");
		auto shoesWalkAnimations = AnimationFactory::CreateAnimations(shoesTextureID, startFrame, endFrame, animationSpeed, spriteSize);
		for (auto a : *shoesWalkAnimations)
		{
			objWalkAnimationGroups[a.first].AddAnimation(a.second);
		}
		auto shoesIdleAnimations = AnimationFactory::CreateAnimations(shoesTextureID, 0, 0, 0, spriteSize);
		for (auto a : *shoesIdleAnimations)
		{
			objIdleAnimationGroups[a.first].AddAnimation(a.second);
		}

		objSprite->AddAnimation(ANIMATION_STATE::WALK_UP, objWalkAnimationGroups[MOVEMENT_DIRECTION::UP]);
		objSprite->AddAnimation(ANIMATION_STATE::WALK_LEFT, objWalkAnimationGroups[MOVEMENT_DIRECTION::LEFT]);
		objSprite->AddAnimation(ANIMATION_STATE::WALK_DOWN, objWalkAnimationGroups[MOVEMENT_DIRECTION::DOWN]);
		objSprite->AddAnimation(ANIMATION_STATE::WALK_RIGHT, objWalkAnimationGroups[MOVEMENT_DIRECTION::RIGHT]);

		objSprite->AddAnimation(ANIMATION_STATE::IDLE_UP, objIdleAnimationGroups[MOVEMENT_DIRECTION::UP]);
		objSprite->AddAnimation(ANIMATION_STATE::IDLE_LEFT, objIdleAnimationGroups[MOVEMENT_DIRECTION::LEFT]);
		objSprite->AddAnimation(ANIMATION_STATE::IDLE_DOWN, objIdleAnimationGroups[MOVEMENT_DIRECTION::DOWN]);
		objSprite->AddAnimation(ANIMATION_STATE::IDLE_RIGHT, objIdleAnimationGroups[MOVEMENT_DIRECTION::RIGHT]);

		objSprite->SetSortOrder(100);

		follower->m_transform->SetPosition(m_dungeon.GetRandomFloorLocation());

		auto collider2 = follower->AddComponent<C_BoxCollider>();
		collider2->SetCollidable(bodyWalkAnimations->at(MOVEMENT_DIRECTION::DOWN).m_sprite.getGlobalBounds());
		collider2->SetLayer(CollisionLayer::Followers);
		collider2->SetTrigger(true);

		follower->AddComponent<C_Velocity>();
		follower->AddComponent<C_Pathfinding>();
		follower->AddComponent<C_DirectionalAnimation>();
		follower->AddComponent<C_Direction>();

		follower->AddComponent<C_Seperation>();
		follower->AddComponent<C_BehaviorApplier>();

		Object::Add(follower);
	}



	auto context = m_stateManager->m_context;
	context->m_level = &m_dungeon;
	context->m_player = m_player;
	context->m_pathFinder = &m_pathFinder;

	Raycast::Initialise(context);
	Debug::Initialise(*context);
	Input::Initialise();
}

void S_Game::OnDestroy()
{

}

void S_Game::Activate() {}

void S_Game::Deactivate() {}

void S_Game::Update(float deltaTime)
{
	Input::EarlyUpdate();

	m_collisions.Resolve();

	Object::UpdateAll(deltaTime);

	if (Input::IsKeyDown(Input::KEY::KEY_ESC))
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

	Input::LateUpdate();
}


