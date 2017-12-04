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

void Object::Awake()
{
	for (auto& c : m_components)
	{
		c->Awake();
	}
}

void Object::Start()
{
	for (auto& c : m_components)
	{
		c->Start();
	}
}

void Object::Update(float timeDelta)
{
	for (const auto& component : m_updatables)
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

void Object::OnCollisionEnter(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnCollisionEnter(*this, other);
	}
}

void Object::OnCollisionStay(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnCollisionStay(*this, other);
	}
}

void Object::OnCollisionExit(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnCollisionExit(*this, other);
	}
}


void Object::OnTriggerEnter(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnTriggerEnter(*this, other);
	}
}

void Object::OnTriggerStay(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnTriggerStay(*this, other);
	}
}

void Object::OnTriggerExit(std::shared_ptr<Object> other)
{
	for (const auto& component : m_collidables)
	{
		component->OnTriggerExit(*this, other);
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

//TODO: cache?
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

std::vector<std::shared_ptr<Object>>& Object::GetObjects()
{
	return m_objects;
}

void Object::ProcessNewObjects()
{
	// Move newly created objects to global list.
	if (m_newObjects.size() > 0)
	{
		AwakeNew();
		StartNew();

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

void Object::AwakeNew()
{
	for (const auto& obj : m_newObjects)
	{
		obj->Awake();
	}
}

void Object::StartNew()
{
	for (const auto& obj : m_newObjects)
	{
		obj->Start();
	}
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