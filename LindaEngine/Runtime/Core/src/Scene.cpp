#include "Scene.h"
#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "Renderer.h"
#include "Material.h"
#include "MaterialManager.h"
#include "FBXManager.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "TransformSystem.h"
#include "UUID.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

Entity* Scene::CreateEntity(const char* name, bool active)
{
	Ref<Entity> e = CreateRef<Entity>(name, active);
	_entitys.push_back(e);
	return e.get();
}

Entity* Scene::InstantiateCube(Transform* parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Cube.FBX", parent);
}

Entity* Scene::InstantiateSphere(Transform* parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Sphere.FBX", parent);
}

Entity* Scene::InstantiatePlane(Transform* parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Plane.FBX", parent);
}

Entity* Scene::InstantiatePrefab(std::string path, Transform* parent)
{
	return DeserializePrefab(path, parent);
}

Entity* Scene::InstantiateFBX(std::string path, Transform* parent)
{
	Ref<FBXResources> res = FBXManager::GetFBX(path);

	return CreateEntityFromFBX(res, parent);
}

void Scene::DestroyEntity(Entity* entity)
{
	_dirtyEntitys.push_back(entity);
}

void Scene::DestroyEntityImmediately(Entity* entity)
{
	Transform* trans = entity->GetTransform();
	trans->SetParent(nullptr);
	DestroyEntityIncludeChild(entity);
}

Entity* Scene::GetEntity(const char* name)
{
	for (auto& e : _entitys) {
		if (e->GetName() != name)
			continue;
		return e.get();
	}
	return nullptr;
}

Entity* Scene::GetEntity(int id)
{
	for (auto& e : _entitys) {
		if (e->_entityID != id)
			continue;
		return e.get();
	}
	return nullptr;
}

void Scene::DestroyEntity()
{
	for (auto& e : _dirtyEntitys) 
	{
		Transform* trans = e->GetTransform();
		trans->SetParent(nullptr);
		DestroyEntityIncludeChild(e);
	}
	_dirtyEntitys.clear();
}

void Scene::UpdateEntityComponents()
{
	for (auto& e : _entitys)
	{
		e->RemoveDirtyComponents();
		e->AddBehaviorsToSystem();
	}
}

void Scene::DestroyEntityIncludeChild(Entity* entity)
{
	Transform* trans = entity->GetTransform();
	const std::list<Transform*>& children = trans->GetChildren();
	for (auto& t : children)
	{
		DestroyEntityIncludeChild(&(t->GetEntity()));
	}

	for (auto iter = _entitys.begin(); iter != _entitys.end(); ++iter) {
		if ((*iter)->GetID() != entity->GetID())
			continue;

		entity->Destroy();
		_entitys.erase(iter);
		break;
	}
}

Entity* Scene::CreateEntityFromFBX(Ref<FBXResources> res, Transform* parent)
{
	Entity* entity = CreateEntity(res->name.c_str());

	Transform* transform = entity->GetTransform();
	transform->SetParent(parent);
	transform->SetLocalPosition(res->localPosition);
	transform->SetLocalRotation(res->localRotation);
	transform->SetLocalScale(res->localScale);

	if (nullptr != res->mesh)
	{
		MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
		renderer->SetMesh(res->mesh);
		int index = 0;
		for (auto& data : res->mesh->GetAllMeshData())
		{
			renderer->AddMaterial(index++, MaterialManager::GetMaterial("Assets/Materials/test.mat"));
		}
	}
	for (auto& node : res->children)
	{
		CreateEntityFromFBX(node, transform);
	}

	transform->SetParent(parent);
	return entity;
}

void Scene::Destroy()
{
	std::vector<Ref<Entity>> temp;

	for (auto& entity : _entitys)
	{
		if (entity->GetDontDestory())
		{
			temp.push_back(entity);
			continue;
		}
		entity->Destroy();
	}
	_entitys.clear();
	_index = -1;
	_path = "";

	for (auto& entity : temp)
	{
		_entitys.push_back(entity);
	}
}

void Scene::SetSkyboxMaterial(Ref<Material> material)
{
	Renderer::SetSkyboxMaterial(material);
}

void Scene::ResetUUID(std::vector<Ref<Entity>>& entitys)
{
	for (auto& go : entitys)
	{
		std::string uuid = UUID::Get();
		go->SetUUID(uuid);
		for (auto& child : go->GetTransform()->GetChildren())
		{
			child->SetParentID(uuid);
		}
	}
}

void Scene::ResetSceneUUID()
{
	ResetUUID(_entitys);
}

void Scene::DuplicateEntity(Entity* entity)
{

}

bool Scene::Serialize()
{
	YAML::Emitter out;
	YamlSerializer::out = &out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FileName" << YAML::Value << _path;

	out << YAML::Key << "SkyboxMaterial" << YAML::Value << Renderer::GetSkyboxMaterial()->GetPath();

	out << YAML::Key << "Entitys";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& entity : _entitys)
	{
		entity->Serialize();
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(_path);
		fout << out.c_str();
		YamlSerializer::out = nullptr;
		return true;
	}
	catch (const std::exception&)
	{
		YamlSerializer::out = nullptr;
		std::cout << "Scene::Serialize Error" << _path << "\n" << std::endl;
		return false;
	}
}

bool Scene::Deserialize(YAML::Node& node)
{
	auto scene = node["Scene"];
	if (!scene)
		return false;

	_path = scene["FileName"].as<std::string>();

	SetSkyboxMaterial(MaterialManager::GetMaterial(scene["SkyboxMaterial"].as<std::string>().c_str()));

	auto entitys = scene["Entitys"];
	for (auto entity : entitys)
	{
		std::string entityName = entity["Name"].as<std::string>();
		Ref<Entity> e = CreateRef<Entity>(entityName.c_str(), entity["Active"].as<bool>());
		e->Deserialize(entity);
		_entitys.push_back(e);
	}

	return true;

}

bool Scene::SerializePrefab(std::string path, Entity* entity)
{
	YAML::Emitter out;
	YamlSerializer::out = &out;
	out << YAML::BeginMap;

	out << YAML::Key << "Root";
	out << YAML::Value << entity->GetUUID();

	out << YAML::Key << "Entitys";
	out << YAML::Value << YAML::BeginSeq;
	entity->Serialize();
	SerializeHierarchyPrefab(entity);
	out << YAML::EndSeq;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(path);
		fout << out.c_str();
		YamlSerializer::out = nullptr;
		return true;
	}
	catch (const std::exception&)
	{
		YamlSerializer::out = nullptr;
		std::cout << "Scene::SerializePrefab Error" << path << "\n" << std::endl;
		return false;
	}

}

void Scene::SerializeHierarchyPrefab(Entity* entity)
{
	for (auto& trans : entity->GetTransform()->GetChildren())
	{
		Entity& go = trans->GetEntity();
		go.Serialize();
		SerializeHierarchyPrefab(&go);
	}
}

Entity* Scene::DeserializePrefab(std::string path, Transform* parent)
{
	YAML::Node data;
	try
	{
		std::vector<Transform*> list;
		std::vector<Ref<Entity>> entityArray;

		data = YAML::LoadFile(path);
		
		std::string root = data["Root"].as<std::string>();
		auto entitys = data["Entitys"];
		for (auto entity : entitys)
		{
			std::string entityName = entity["Name"].as<std::string>();
			Ref<Entity> e = CreateRef<Entity>(entityName.c_str(), entity["Active"].as<bool>());
			e->Deserialize(entity);
			_entitys.push_back(e);
			list.push_back(e->GetTransform());
			entityArray.push_back(e);
		}

		Transform* ret = nullptr;
		for (auto& go : list)
		{
			if (nullptr == go)
				continue;

			if (go->GetEntity().GetUUID() == root)
			{
				go->SetParent(parent);
				ret = go;
			}
			else
			{
				go->SetParent(TransformSystem::Get(list, go->GetParentID()));
			}
			go->CalculateLocalMatrix();
		}
		ResetUUID(entityArray);
		if (nullptr != ret)
			return &ret->GetEntity();
		else
			return nullptr;
	}
	catch (const std::exception&)
	{
		std::cout << "Scene::DeserializePrefab Error" << path << "\n" << std::endl;
		return nullptr;
	}

}
