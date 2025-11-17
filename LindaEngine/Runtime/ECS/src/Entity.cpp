#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "UUID.h"
#include "ComponentFactory.h"
#include "Behavior.h"
#include "BehaviorSystem.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

bool Entity::_isPlaying = true;

Entity::Entity(const char* name, bool active)
{
	_name = name;
	_active = active;
	_activeDirty = false;
	_uuid = UUID::Get();

	Ref<Transform> c = CreateRef<Transform>(*this);
	_components.push_back(c);
	OnComponentAdded(c.get());
	_transform = c.get();
}

Entity::~Entity()
{
	Destroy();
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
	_activeDirty = true;
	UpdateChildrenDirty(_transform);
}

bool Entity::IsActive()
{
	const Transform* parent = _transform->GetParent();
	return _active && (parent != nullptr ? parent->GetEntity().IsActive() : true);
}

void Entity::Destroy()
{
	for (auto& com : _components) {
		com->Destroy();
		OnComponentRemoved(com.get());
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
	Behavior* pointer = dynamic_cast<Behavior*>(com);
	if (nullptr != pointer)
	{
		BehaviorSystem::Add(pointer);
		return;
	}
}

void Entity::OnComponentRemoved(Component* com)
{
	Behavior* pointer = dynamic_cast<Behavior*>(com);
	if (nullptr != pointer)
	{
		BehaviorSystem::Remove(pointer);
		return;
	}
}

void Entity::UpdateChildrenDirty(Transform* parent)
{
	for (auto& child : parent->GetChildren())
	{
		child->GetEntity()._activeDirty = true;
		UpdateChildrenDirty(child);
	}
}

bool Entity::IsDirty()
{
	return _activeDirty;
}

void Entity::ClearDirty()
{
	_activeDirty = false;
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
