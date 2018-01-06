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

	//TODO: move this a text/data file.
	const int animationSpeed = 9;
	const int swingAnimationSpeed = 12;
	const int startFrame = 1;
	const int endFrame = 9;
	const int spriteSize = 64;

	/*******************
	 Player setup start.
	********************/
	{
		m_player = std::make_shared<Object>(*m_stateManager->m_context);

		m_player->m_transform->SetPosition(m_dungeon.GetRandomFloorLocation());
		m_player->GetComponent<C_Tag>()->Set(PLAYER_TAG);

		m_player->AddComponent<C_Velocity>();

		auto controller = m_player->AddComponent<C_KeyboardController>();
		controller->SetMovementSpeed(120);

		auto playerSprite = m_player->AddComponent<C_AnimatedSprite>();

		//TODO: Need to be able to easily swap out animations e.g. you want to be able to change armour/sword parts easily.

		AnimationGroup walkAnimations;
		AnimationGroup idleAnimations;
		AnimationGroup meleeSwingAnimations;

		/* Body Animatons */
		int walkTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/walk.png");
		auto bodyWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, walkTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *bodyWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto bodyIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, walkTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *bodyIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		/* Hair Animations */
		int hairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/walk.png");
		auto hairWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, hairTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *hairWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto hairIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, hairTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *hairIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		/* Pants Animations */
		int pantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/walk.png");
		auto pantsWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, pantsTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *pantsWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto pantsIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, pantsTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *pantsIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}


		/* Shirt Animations */
		int shirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/walk.png");
		auto shirtWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, shirtTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *shirtWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto shirtIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, shirtTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *shirtIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		/* Shoes Animation */
		int shoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/walk.png");
		auto shoesWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, shoesTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *shoesWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto shoesIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, shoesTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *shoesIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		/* Sword Animations */
		int swingBodyTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/swing.png");
		auto swingBodyAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, swingBodyTextureID, 0, 5, swingAnimationSpeed, spriteSize, false);
		for (auto a : *swingBodyAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swordTextureID = TextureManager::AddTexture("../resources/weapons/sword.png");
		auto swordAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SWORD, swordTextureID, 0, 5, swingAnimationSpeed, 128, false);
		for (auto a : *swordAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingShoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/swing.png");
		auto swingShoesAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, swingShoesTextureID, 0, 5, swingAnimationSpeed, spriteSize, false);
		for (auto a : *swingShoesAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingHairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/swing.png");
		auto swingHairAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, swingHairTextureID, 0, 5, swingAnimationSpeed, spriteSize, false);
		for (auto a : *swingHairAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingPantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/swing.png");
		auto swingPantsAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, swingPantsTextureID, 0, 5, swingAnimationSpeed, spriteSize, false);
		for (auto a : *swingPantsAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingshirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/swing.png");
		auto swingShirtAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, swingshirtTextureID, 0, 5, swingAnimationSpeed, spriteSize, false);
		for (auto a : *swingShirtAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		playerSprite->AddAnimation(ANIMATION_STATE::IDLE, idleAnimations);
		playerSprite->AddAnimation(ANIMATION_STATE::WALK, walkAnimations);
		playerSprite->AddAnimation(ANIMATION_STATE::SWING, meleeSwingAnimations);


		playerSprite->SetSortOrder(1000);

		m_player->AddComponent<C_DirectionalAnimation>();

		auto collider1 = m_player->AddComponent<C_BoxCollider>();
		collider1->SetCollidable(sf::FloatRect(bodyWalkAnimations->at(MOVEMENT_DIRECTION::DOWN).GetSprite().getGlobalBounds()));
		collider1->SetLayer(CollisionLayer::Player);

		//m_player->AddComponent<C_CollidableTest>();
		m_player->AddComponent<C_RaycastTest>();

		//m_view.setCenter(m_player->m_transform->GetPosition());
		m_player->AddComponent<C_Camera>();
		m_player->AddComponent<C_MeleeAttack>();
		m_player->AddComponent<C_Direction>();

		Object::Add(m_player);

		auto label = m_player->AddComponent<C_DebugPositionLabel>();
		label->SetOffset(sf::Vector2f(0.f, 0.f));
		label->SetSortOrder(1010);
	}
	/*******************
	 Player setup end.
	********************/

	for (int i = 0; i < 10; ++i)
	{
		auto follower = std::make_shared<Object>(*m_stateManager->m_context);

		follower->m_tag->Set(FOLLOWER_TAG);

		auto objSprite = follower->AddComponent<C_AnimatedSprite>();
		AnimationGroup walkAnimations;
		AnimationGroup idleAnimations;
		AnimationGroup meleeSwingAnimations;

		int walkTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/walk.png");
		auto bodyWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, walkTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *bodyWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto bodyIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, walkTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *bodyIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		int hairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/walk.png");
		auto hairWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, hairTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *hairWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto hairIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, hairTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *hairIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		int pantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/walk.png");
		auto pantsWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, pantsTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *pantsWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto pantsIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, pantsTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *pantsIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		int shirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/walk.png");
		auto shirtWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, shirtTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *shirtWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto shirtIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, shirtTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *shirtIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		int shoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/walk.png");
		auto shoesWalkAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, shoesTextureID, startFrame, endFrame, animationSpeed, spriteSize, true);
		for (auto a : *shoesWalkAnimations)
		{
			walkAnimations.AddAnimation(a.first, a.second);
		}
		auto shoesIdleAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, shoesTextureID, 0, 0, 0, spriteSize, true);
		for (auto a : *shoesIdleAnimations)
		{
			idleAnimations.AddAnimation(a.first, a.second);
		}

		int swingBodyTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "body/male_1/swing.png");
		auto swingBodyAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::BODY, swingBodyTextureID, 0, 5, animationSpeed, spriteSize, false);
		for (auto a : *swingBodyAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swordTextureID = TextureManager::AddTexture("../resources/weapons/sword.png");
		auto swordAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SWORD, swordTextureID, 0, 5, animationSpeed, 128, false);
		for (auto a : *swordAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingShoesTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shoes/brown/swing.png");
		auto swingShoesAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHOES, swingShoesTextureID, 0, 5, animationSpeed, spriteSize, false);
		for (auto a : *swingShoesAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingHairTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "hair/blonde/swing.png");
		auto swingHairAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::HAIR, swingHairTextureID, 0, 5, animationSpeed, spriteSize, false);
		for (auto a : *swingHairAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingPantsTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "pants/green/swing.png");
		auto swingPantsAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::PANTS, swingPantsTextureID, 0, 5, animationSpeed, spriteSize, false);
		for (auto a : *swingPantsAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		int swingshirtTextureID = TextureManager::AddTexture(BODY_PARTS_ROOT_FOLDER + "shirt/leather_white/swing.png");
		auto swingShirtAttackAnimations = AnimationFactory::CreateAnimations(SPRITE_TYPE::SHIRT, swingshirtTextureID, 0, 5, animationSpeed, spriteSize, false);
		for (auto a : *swingShirtAttackAnimations)
		{
			meleeSwingAnimations.AddAnimation(a.first, a.second);
		}

		objSprite->AddAnimation(ANIMATION_STATE::IDLE, idleAnimations);
		objSprite->AddAnimation(ANIMATION_STATE::WALK, walkAnimations);
		objSprite->AddAnimation(ANIMATION_STATE::SWING, meleeSwingAnimations);

		objSprite->SetSortOrder(100);

		follower->m_transform->SetPosition(m_dungeon.GetRandomFloorLocation());

		auto collider2 = follower->AddComponent<C_BoxCollider>();
		collider2->SetCollidable(bodyWalkAnimations->at(MOVEMENT_DIRECTION::DOWN).GetSprite().getGlobalBounds());
		collider2->SetLayer(CollisionLayer::Followers);
		collider2->SetTrigger(true);

		follower->AddComponent<C_Velocity>();
		follower->AddComponent<C_Pathfinding>();
		follower->AddComponent<C_DirectionalAnimation>();
		follower->AddComponent<C_Direction>();

		follower->AddComponent<C_Seperation>();
		follower->AddComponent<C_BehaviorApplier>();

		follower->AddComponent<C_Health>();
		follower->AddComponent<C_KnockBack>();

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

	m_fps.Draw(*window, deltaTime);
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


