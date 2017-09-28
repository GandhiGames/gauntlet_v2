#include "Object.h"


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