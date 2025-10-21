#include "Entity.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "UUID.h"
#include "ComponentFactory.h"
#include "Renderer.h"
#include "RendererSystem.h"

#include "ComponentSystem2.h"

#include <fstream>
#include <iostream>
#include <string>

#define COMPONENTADDED(className, pointerSrc, comSystem, pointer, otherCode) \
className* pointer = dynamic_cast<className*>(pointerSrc); \
if (nullptr != pointer) \
{ \
	comSystem::Add(pointer); \
	otherCode \
	return; \
} 

#define COMPONENTREMOVED(className, pointerSrc, comSystem, pointer, otherCode) \
className* pointer = dynamic_cast<className*>(pointerSrc); \
if (nullptr != pointer) \
{ \
	comSystem::Remove(pointer); \
	otherCode \
	return; \
} 

using namespace LindaEngine;

bool Entity::_isPlaying = true;

Entity::Entity(const char* name, bool active)
{
	_name = name;
	_active = active;
	_uuid = UUID::Get();

	Ref<Transform> c = CreateRef<Transform>(*this);
	_components.push_back(c);
	OnComponentAdded(c.get());
	_transform = c.get();

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
	_transform = nullptr;
}

void Entity::TransformDirty()
{
	for (auto& com : _components) {
		com->TransformDirty();
	}
}

Transform* Entity::GetTransform()
{
	return _transform;
}

void Entity::OnComponentAdded(Component* com)
{
	//Transform* trans = dynamic_cast<Transform*>(com);
	//if (nullptr != trans)
	//{
	//	TransformSystem::Add(trans);
	//	return;
	//}

	//Camera* camera = dynamic_cast<Camera*>(com);
	//if (nullptr != camera)
	//{
	//	CameraSystem::Add(camera);
	//	_transform->OnCameraAdded();
	//	return;
	//}

	//Renderer* renderer = dynamic_cast<Renderer*>(com);
	//if (nullptr != renderer)
	//{
	//	RendererSystem::Add(renderer);
	//	return;
	//}

	COMPONENTADDED(Transform, com, TransformSystem, trans, ;)
	COMPONENTADDED(Camera, com, CameraSystem, camera, _transform->OnCameraAdded();)
	COMPONENTADDED(Renderer, com, RendererSystem, renderer, ;)
}

void Entity::OnComponentRemoved(Component* com)
{
	COMPONENTREMOVED(Transform, com, TransformSystem, trans, ;)
	COMPONENTREMOVED(Camera, com, CameraSystem, camera, _transform->OnCameraRemoved();)
	COMPONENTREMOVED(Renderer, com, RendererSystem, renderer, ;)
	//Transform* trans = dynamic_cast<Transform*>(com);
	//if (nullptr != trans)
	//{
	//	TransformSystem::Remove(trans);
	//	return;
	//}

	//Camera* camera = dynamic_cast<Camera*>(com);
	//if (nullptr != camera)
	//{
	//	CameraSystem::Remove(camera);
	//	_transform->OnCameraRemoved();
	//	return;
	//}

	//Renderer* renderer = dynamic_cast<Renderer*>(com);
	//if (nullptr != renderer)
	//{
	//	RendererSystem::Remove(renderer);
	//	return;
	//}
}

bool Entity::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << _name;
	out << YAML::Key << "ID" << YAML::Value << _uuid;
	out << YAML::Key << "Active" << YAML::Value << _active;
	out << YAML::Key << "Components";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& com : _components)
	{
		com->Serialize();
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	return true;
}

bool Entity::Deserialize(YAML::Node& node)
{
	auto components = node["Components"];
	if (!components)
		return false;

	_uuid = node["ID"].as<std::string>();

	for (auto com : components)
	{
		std::string comName = com["Name"].as<std::string>();
		
		if (comName == "Transform")
		{
			_transform->Deserialize(com);
		}
		else
		{
			Ref<Component> pointer = ComponentFactory::CreateComponent(comName, *this, com["enable"].as<bool>());
			pointer->Deserialize(com);
			_components.push_back(pointer);
			OnComponentAdded(pointer.get());
		}
	}

	return true;
}
