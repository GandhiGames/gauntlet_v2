#include "Object.h"

std::vector<std::shared_ptr<Object>> Object::m_objects = {};
std::vector<std::shared_ptr<Object>> Object::m_newObjects = {};

// Transform component automatically added to all new objects.
Object::Object(SharedContext& context) : m_queuedForRemoval(false), m_context(context)
{
	m_transform = AddComponent<C_Transform>();
	m_instanceID = AddComponent<C_InstanceID>();
	m_tag = AddComponent<C_Tag>();
}

void Object::Update(float timeDelta)
{
	//TODO: cache updateables
	auto updateables = GetComponents<C_Updateable>();
	for (const auto& component : updateables)
	{
		component->Update(timeDelta);
	}
}

void Object::LateUpdate(float timeDelta)
{
	for (const auto& component : m_updatables)
	{
		component->LateUpdate(timeDelta);
	}
}

//TODO: do we need deltaTime for draw anymore?
void Object::Draw(sf::RenderWindow &window, float timeDelta)
{
	for (const auto& component : m_drawables)
	{
		component->Draw(window, timeDelta);
	}
}

void Object::OnCollisionEnter(Object* other)
{
	for (const auto& component : m_collidables)
	{
		component->OnCollisionEnter(this, other);
	}
}

void Object::OnCollisionExit(Object* other)
{
	for (const auto& component : m_collidables)
	{
		component->OnCollisionExit(this, other);
	}
}


void Object::Destroy()
{
	auto parent = m_transform->GetParent();

	if (parent)
	{
		parent->RemoveChild(m_transform);
	}

	auto children = m_transform->GetChildren();

	if (children.size() > 0)
	{
		for (auto& child : children)
		{
			child->m_owner->Destroy();
		}
	}

	m_queuedForRemoval = true;
}

bool Object::IsQueuedForRemoval() const
{
	return m_queuedForRemoval;
}

void Object::Add(std::shared_ptr<Object> object)
{
	m_newObjects.push_back(object);
}

std::vector<std::shared_ptr<Object>> Object::GetObjectsWithTag(const std::string& tag)
{
	std::vector<std::shared_ptr<Object>> retObjects;

	for (auto& o : m_objects)
	{
		if (o->m_tag->Get() == tag)
		{
			retObjects.push_back(o);
		}
	}

	return retObjects;
}

void Object::ProcessNewObjects()
{
	// Move newly created objects to global list.
	if (m_newObjects.size() > 0)
	{
		for (auto& obj : m_newObjects)
		{
			Object::m_objects.push_back(obj);
		}
		m_newObjects.clear();

		// Sort them so they are drawn in correct order.
		std::sort(Object::m_objects.begin(), Object::m_objects.end(), by_draw_order());
	}
}

std::vector<std::shared_ptr<Object>> Object::GetNewObjects()
{
	return m_newObjects;
}

void Object::UpdateAll(float deltaTime)
{
	for (const auto& obj : m_objects)
	{
		obj->Update(deltaTime);
	}
}

void Object::LateUpdateAll(float deltaTime)
{
	for (const auto& obj : Object::m_objects)
	{
		obj->LateUpdate(deltaTime);
	}



}

void Object::DrawAll(sf::RenderWindow& window, float deltaTime)
{
	for (const auto& obj : Object::m_objects)
	{
		obj->Draw(window, deltaTime);
	}
}

bool Object::ProcessRemovals()
{
	//Process removals;
	bool objRemoved = false;
	auto objIterator = Object::m_objects.begin();
	while (objIterator != Object::m_objects.end())
	{
		auto obj = **objIterator;

		bool remove = obj.IsQueuedForRemoval();

		if (remove)
		{
			objRemoved = true;
			objIterator = Object::m_objects.erase(objIterator);
		}
		else
		{
			++objIterator;
		}
	}

	return objRemoved;
}