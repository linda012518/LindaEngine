#include "Entity.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "UUID.h"
#include "ComponentFactory.h"

#include <fstream>
#include <iostream>
#include <string>

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
			//glm::vec3 pos = com["worldPosition"].as<glm::vec3>();
			//glm::vec3 scale = com["worldScale"].as<glm::vec3>();
			//glm::quat rotation = com["worldRotation"].as<glm::quat>();
			//std::string parent = com["parent"].as<std::string>();
			//_transform->SetWorldPosition(pos);
			//_transform->SetWorldScale(scale);
			//_transform->SetWorldRotation(rotation);
			//_transform->SetParentID(parent);
			_transform->Deserialize(com);
		}
		else
		{
			ComponentFactory::CreateComponent(comName, *this, com["enable"].as<bool>())->Deserialize(com);
		}
	}

	return true;
}
