#include "Entity.h"

#include "Transform.h"
#include "TransformSystem.h"

#include <string.h>

using namespace LindaEngine;

bool Entity::_isPlaying = true;

Entity::Entity(const char* name, bool active)
{
	_name = name;
	_active = active;

	Ref<Transform> c = CreateRef<Transform>(*this);
	_components.push_back(c);
	OnComponentAdded(c.get());

	std::cout << "	Entity" << _selfID << std::endl;
}

Entity::~Entity()
{
	Destroy();
	std::cout << "	~Entity" << _selfID << std::endl;
}

void Entity::SetName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::GetName() const
{
	return _name;
}

void Entity::SetActive(bool active)
{
	if (_active == active)
		return;
	_active = active;

	for (auto& com : _components) {
		com->SetDirty();
	}
}

bool Entity::IsActive()
{
	const Transform* parent = GetComponent<Transform>()->GetParent();
	return _active && (parent != nullptr ? parent->GetEntity().IsActive() : true);
}

void Entity::Destroy()
{
	for (auto& com : _components) {
		com->OnDestroy();
		OnComponentRemoved(com.get());
	}
	_components.clear();
}

void Entity::OnComponentAdded(Component* com)
{
	auto ptr = dynamic_cast<Transform*>(com);
	if (nullptr != ptr)
		TransformSystem::Add(ptr);
}

void Entity::OnComponentRemoved(Component* com)
{
	auto ptr = dynamic_cast<Transform*>(com);
	if (nullptr != ptr)
		TransformSystem::Remove(ptr);
}

