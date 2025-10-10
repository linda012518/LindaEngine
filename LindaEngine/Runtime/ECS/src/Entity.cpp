#include "Entity.h"
#include "LifeCycleFuncSystem.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"

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
}

bool Entity::IsActive()
{
	const Transform* parent = GetComponent<Transform>()->GetParent();
	return _active && (parent != nullptr ? parent->GetEntity().IsActive() : true);
}

void Entity::Destroy()
{
	for (auto& com : _components) {
		com->Destroy();
		OnComponentRemoved(com.get());
		//LifeCycleFuncSystem::RemoveComponent(com.get());
	}
	_components.clear();
}

void Entity::TransformDirty()
{
	for (auto& com : _components) {
		com->TransformDirty();
	}
}

void Entity::OnComponentAdded(Component* com)
{
	//LifeCycleFuncSystem::AddComponentAwake(com);
	//LifeCycleFuncSystem::AddComponentStart(com);

	Transform* trans = dynamic_cast<Transform*>(com);
	if (nullptr != trans)
		TransformSystem::Add(trans);

	Camera* camera = dynamic_cast<Camera*>(com);
	if (nullptr != camera)
		CameraSystem::Add(camera);
}

void Entity::OnComponentRemoved(Component* com)
{
	Transform* trans = dynamic_cast<Transform*>(com);
	if (nullptr != trans)
		TransformSystem::Remove(trans);

	Camera* camera = dynamic_cast<Camera*>(com);
	if (nullptr != camera)
		CameraSystem::Remove(camera);
}

