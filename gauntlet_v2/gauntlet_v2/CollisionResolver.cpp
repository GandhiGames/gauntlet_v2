#include "CollisionResolver.h"

CollisionResolver::CollisionResolver()
{
	COLLISION_LAYERS.insert(std::make_pair(CollisionLayer::Default, Bitmask(0)));
	COLLISION_LAYERS.insert(std::make_pair(CollisionLayer::Player, Bitmask(0)));
	COLLISION_LAYERS.insert(std::make_pair(CollisionLayer::Followers, Bitmask(0)));

	/*
	0 - 0
	0 - 1
	1 - 0
	1 - 2

	colliding: 0 + 0
	colliding: 0 + 1
	colliding: 1 + 0
	colliding: 2 + 0
	colliding: 1 + 2
	colliding: 2 + 1
	colliding: 2 + 2

	*/
}


CollisionResolver::~CollisionResolver()
{
}

void CollisionResolver::ProcessRemovals()
{
	ProcessCollisionExitDueToRemovals();

	for (auto& layer : m_collidables)
	{
		auto itr = layer.second.begin();
		while (itr != layer.second.end())
		{
			if ((*itr)->IsQueuedForRemoval())
			{
				itr = layer.second.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}

void CollisionResolver::ProcessNewObjects()
{
	auto objects = Object::GetNewObjects();

	if (objects.size() == 0)
	{
		return;
	}

	for (auto& obj : objects)
	{
		auto collider = obj->GetComponent<C_Collider2D>();
		if (collider)
		{
			CollisionLayer layer = collider->GetLayer();

			auto itr = m_collidables.find(layer);

			if (itr != m_collidables.end())
			{
				m_collidables[layer].push_back(obj);
			}
			else
			{
				std::vector<std::shared_ptr<Object>> objs;
				objs.push_back(obj);

				m_collidables.insert(std::make_pair(layer, objs));
			}
		}
	}
}

void CollisionResolver::Resolve()
{
	ProcessCollisionExits();

	for (auto maps = m_collidables.begin(); maps != m_collidables.end(); ++maps)
	{
		for (auto maps2 = maps; maps2 != m_collidables.end(); ++maps2)
		{
			bool firstSecond = COLLISION_LAYERS[maps->first].GetBit((int)maps2->first);
			bool secondFirst = COLLISION_LAYERS[maps2->first].GetBit((int)maps->first);
			if (firstSecond)
			{
				/*
				char result2[100];
				sprintf_s(result2, "\n colliding: %d + %d", maps->first, maps2->first);
				printf(result2);
				*/

				ProcessCollisions(maps->second, maps2->second);
			}

			if (secondFirst && ((firstSecond && maps != maps2) || !firstSecond))
			{
				/*
				char result[100];
				sprintf_s(result, "\n colliding: %d + %d", maps2->first, maps->first);
				printf(result);
				*/

				ProcessCollisions(maps2->second, maps->second);
			}

		}


	}
}

void CollisionResolver::ProcessCollisions(std::vector<std::shared_ptr<Object>>& first, std::vector<std::shared_ptr<Object>>& second)
{
	for (auto itr = first.begin(); itr != first.end(); ++itr)
	{
		auto collider1 = (*itr)->GetComponent<C_BoxCollider>();
		auto collidable1 = collider1->GetCollidable();

		for (auto itr2 = second.begin(); itr2 != second.end(); ++itr2)
		{
			if ((*itr)->m_instanceID->Get() == (*itr2)->m_instanceID->Get())
			{
				continue;
			}

			auto collider2 = (*itr2)->GetComponent<C_BoxCollider>();
			auto collidable2 = collider2->GetCollidable();

			if (collidable1.intersects(collidable2))
			{
				auto emplace = m_colliding.insert(std::make_pair(collider1, collider2));

				bool wasEmplaced = emplace.second;

				bool firstIsTrigger = collider1->IsTrigger();
				bool secondIsTrigger = collider2->IsTrigger();

				//OnCollisionEnter
				if (wasEmplaced)
				{
					auto firstCollisionListeners = (*itr)->GetComponents<C_Collidable>();
					auto secondCollisionListeners = (*itr2)->GetComponents<C_Collidable>();

					auto first = collider1->m_owner;
					auto second = collider2->m_owner;

					for (const auto& c : firstCollisionListeners)
					{
						if (firstIsTrigger || secondIsTrigger)
						{
							c->OnTriggerEnter(first, second);
						}
						else
						{
							c->OnCollisionEnter(first, second);
						}
					}

					for (const auto& c : secondCollisionListeners)
					{
						if (firstIsTrigger || secondIsTrigger)
						{
							c->OnTriggerEnter(second, first);
						}
						else
						{
							c->OnCollisionEnter(second, first);
						}
					}
				}

				//Prevent non-trigger colliders from intersecting
				if (!firstIsTrigger && !secondIsTrigger)
				{
					float xDiff = (collidable1.left + (collidable1.width / 2)) - (collidable2.left + (collidable2.width / 2));
					float yDiff = (collidable1.top + (collidable1.height / 2)) - (collidable2.top + (collidable2.height / 2));
					float resolve = 0;

					if (std::abs(xDiff) > std::abs(yDiff))
					{
						if (xDiff > 0)
						{
							resolve = (collidable2.left + collidable2.width) - collidable1.left;
						}
						else
						{
							resolve = -((collidable1.left + collidable1.width) - collidable2.left);
						}

						// The transform with the largest velocity's position is resolved.
						auto move1 = (*itr)->GetComponent<C_Velocity>();
						auto move2 = (*itr2)->GetComponent<C_Velocity>();
						
						if (move1 == nullptr && move2)
						{
							auto transform = (*itr2)->m_transform;
							transform->SetPosition(transform->GetPosition() + sf::Vector2f(resolve, 0.f));
						}
						else if (move2 == nullptr && move1)
						{
							auto transform = (*itr)->m_transform;
							transform->SetPosition(transform->GetPosition() + sf::Vector2f(resolve, 0.f));
						}
						else if (move1 && move2)
						{
							float vel1 = move1->Get().x + move1->Get().y;
							float vel2 = move2->Get().x + move2->Get().y;

							if (vel1 > vel2)
							{
								auto transform = (*itr)->m_transform;
								transform->SetPosition(transform->GetPosition() + sf::Vector2f(resolve, 0.f));
							}
							else
							{
								auto transform = (*itr2)->m_transform;
								transform->SetPosition(transform->GetPosition() + sf::Vector2f(resolve, 0.f));
							}
						}
						else // move1 and move2 are nulptr, so then how did they collide if they're both static?
						{
							printf("CollisionResolver: Two objects collided without movement");
							auto transform = (*itr)->m_transform;
							transform->SetPosition(transform->GetPosition() + sf::Vector2f(resolve, 0.f));
						}

					
					}
					else {
						if (yDiff > 0)
						{
							resolve = (collidable2.top + collidable2.height) - collidable1.top;
						}
						else
						{
							resolve = -((collidable1.top + collidable1.height) - collidable2.top);
						}

						auto transform = (*itr)->m_transform;
						transform->SetPosition(transform->GetPosition() + sf::Vector2f(0.f, resolve));
					}
				}
			}
		}
	}
}

void CollisionResolver::ProcessCollisionExits()
{
	//Remove objects no longer colliding
	auto itr = m_colliding.begin();
	while (itr != m_colliding.end())
	{
		auto pair = *itr;

		Object* first = pair.first->m_owner;
		Object* second = pair.second->m_owner;

		bool firstIsTrigger = first->GetComponent<C_Collider2D>()->IsTrigger();
		bool secondIsTrigger = second->GetComponent<C_Collider2D>()->IsTrigger();

		auto firstCollisionListeners = first->GetComponents<C_Collidable>();
		auto secondCollisionListeners = second->GetComponents<C_Collidable>();

		if (!pair.first->GetCollidable().intersects(pair.second->GetCollidable()))
		{
			//OnCollisionExit
			for (const auto& c : firstCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerExit(first, second);
				}
				else
				{
					c->OnCollisionExit(first, second);
				}
			}

			for (const auto& c : secondCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerExit(second, first);
				}
				else
				{
					c->OnCollisionExit(second, first);
				}
			}

			itr = m_colliding.erase(itr);
		}
		else
		{
			//OnCollisionStay
			for (const auto& c : firstCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerStay(first, second);
				}
				else
				{
					c->OnCollisionStay(first, second);
				}
			}

			for (const auto& c : secondCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerStay(second, first);
				}
				else
				{
					c->OnCollisionStay(second, first);
				}
			}

			++itr;
		}
	}
}

void CollisionResolver::ProcessCollisionExitDueToRemovals()
{
	//Remove objects no longer colliding
	auto itr = m_colliding.begin();
	while (itr != m_colliding.end())
	{
		auto pair = *itr;

		Object* first = pair.first->m_owner;
		Object* second = pair.second->m_owner;

		bool firstIsTrigger = first->GetComponent<C_Collider2D>()->IsTrigger();
		bool secondIsTrigger = second->GetComponent<C_Collider2D>()->IsTrigger();

		auto firstCollisionListeners = first->GetComponents<C_Collidable>();
		auto secondCollisionListeners = second->GetComponents<C_Collidable>();

		if (first->IsQueuedForRemoval() || second->IsQueuedForRemoval())
		{
			//OnCollisionExit
			for (const auto& c : firstCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerExit(first, second);
				}
				else
				{
					c->OnCollisionExit(first, second);
				}
			}

			for (const auto& c : secondCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerExit(second, first);
				}
				else
				{
					c->OnCollisionExit(second, first);
				}
			}

			itr = m_colliding.erase(itr);
		}
		else
		{
			//OnCollisionStay
			for (const auto& c : firstCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerStay(first, second);
				}
				else
				{
					c->OnCollisionStay(first, second);
				}
			}

			for (const auto& c : secondCollisionListeners)
			{
				if (firstIsTrigger || secondIsTrigger)
				{
					c->OnTriggerStay(second, first);
				}
				else
				{
					c->OnCollisionStay(second, first);
				}
			}

			++itr;
		}
	}
}


