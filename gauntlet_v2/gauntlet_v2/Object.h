#pragma once

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Component.h"
#include "C_Updateable.h"
#include "C_Drawable.h"
#include "C_Collidable.h"
#include "C_Transform.h"
#include "C_InstanceID.h"
#include "C_Tag.h"
#include "C_StaticSprite.h"
#include "C_AnimatedSprite.h"
#include "C_DirectionalAnimation.h"
#include "C_KeyboardController.h"
#include "C_BoxCollider.h"
#include "C_CollidableTest.h"
#include "C_Camera.h"
#include "C_PathFinding.h"
#include "C_RaycastTest.h"
#include "C_MovementBehavior.h"
#include "C_Seperation.h"
#include "C_BehaviorApplier.h"
#include "C_DebugPositionLabel.h"
#include "C_MeleeAttack.h"
#include "C_Damageable.h"
#include "C_Health.h"
#include "C_KnockBack.h"

class Object
{
	friend class SharedContext;
public:
	/**
	* Default constructor.
	*/
	Object(SharedContext& context);

	void Awake();
	void Start();

	void Update(float timeDelta);

	void Draw(sf::RenderWindow &window, float timeDelta);

	void LateUpdate(float timeDelta);

	void OnCollisionEnter(std::shared_ptr<Object> other);
	void OnCollisionStay(std::shared_ptr<Object> other);
	void OnCollisionExit(std::shared_ptr<Object> other);

	void OnTriggerEnter(std::shared_ptr<Object> other);
	void OnTriggerStay(std::shared_ptr<Object> other);
	void OnTriggerExit(std::shared_ptr<Object> other);

	void Destroy();

	bool IsQueuedForRemoval() const;

	/**
	* Attaches a component to the object.
	*/
	template <typename T> std::shared_ptr<T> AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		std::shared_ptr<T> newComponent = std::make_shared<T>(this);

		// Check that we don't already have a component of this type.
		for (auto& exisitingComponent : m_components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				//TODO: create in-game debug/message log.
				std::cout << "Attempting to re-add component: " << newComponent << std::endl;
				return std::dynamic_pointer_cast<T>(exisitingComponent);
			}
		}

		m_components.push_back(newComponent);

		if (std::dynamic_pointer_cast<C_Updateable>(newComponent))
		{
			m_updatables.push_back(std::dynamic_pointer_cast<C_Updateable>(newComponent));
		}

		if (std::dynamic_pointer_cast<C_Drawable>(newComponent))
		{
			m_drawables.push_back(std::dynamic_pointer_cast<C_Drawable>(newComponent));
		}

		if (std::dynamic_pointer_cast<C_Collidable>(newComponent))
		{
			m_collidables.push_back(std::dynamic_pointer_cast<C_Collidable>(newComponent));
		}

		return newComponent;
	};

	/**
	* Gets a component from the object.
	*/
	template <typename T> std::shared_ptr<T> GetComponent()
	{
		// Check that we don't already have a component of this type.
		for (auto& exisitingComponent : m_components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				return std::dynamic_pointer_cast<T>(exisitingComponent);
			}
		}

		return nullptr;
	};

	template <typename T> std::vector<std::shared_ptr<T>> GetComponents()
	{
		std::vector<std::shared_ptr<T>> components;

		for (auto& exisitingComponent : m_components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				components.push_back(std::dynamic_pointer_cast<T>(exisitingComponent));
			}
		}

		return components;
	};

	//static:
	static void Add(std::shared_ptr<Object> object);
	static std::vector<std::shared_ptr<Object>>& GetObjects();
	static std::vector<std::shared_ptr<Object>> GetObjectsWithTag(const std::string& tag);
	static void ProcessNewObjects();
	static std::vector<std::shared_ptr<Object>> GetNewObjects();
	static void UpdateAll(float deltaTime);
	static void LateUpdateAll(float deltaTime);
	static void DrawAll(sf::RenderWindow& window, float deltaTime);
	static bool ProcessRemovals();

public:
	//TODO change to const getter method?
	std::shared_ptr<C_Transform> m_transform;
	std::shared_ptr<C_InstanceID> m_instanceID;
	std::shared_ptr<C_Tag> m_tag;
	SharedContext& m_context;

private:
	struct by_draw_order
	{
		bool operator()(std::shared_ptr<Object> &a, std::shared_ptr<Object> &b)
		{
			auto sprite1 = a->GetComponent<C_Drawable>();
			auto sprite2 = b->GetComponent<C_Drawable>();

			if (!sprite1 && !sprite2)
			{
				return false;
			}

			if (sprite1 && !sprite2)
			{
				return false;
			}

			if (!sprite1 && sprite2)
			{
				return true;
			}

			if (sprite1->GetSortOrder() == sprite2->GetSortOrder())
			{
				return false;
			}
			else if (sprite1->GetSortOrder() < sprite2->GetSortOrder())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	std::vector<std::shared_ptr<Component>> m_components;

	//Maintain seperate lists for components that require updating/drawing.
	std::vector<std::shared_ptr<C_Updateable>> m_updatables;
	std::vector<std::shared_ptr<C_Drawable>> m_drawables;
	std::vector<std::shared_ptr<C_Collidable>> m_collidables;

	bool m_queuedForRemoval;

	//static:
	// Do not add directly to m_objects. Add to m_newObjects instead and they are moved over at frame end.
	static std::vector<std::shared_ptr<Object>> m_newObjects;
	static std::vector<std::shared_ptr<Object>> m_objects;

private:
	static void AwakeNew();
	static void StartNew();
};
