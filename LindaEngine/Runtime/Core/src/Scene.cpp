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
#include "Renderer.h"

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

Entity* Scene::InstantiatePrefab(std::string path)
{
	return nullptr;
}

Entity* Scene::InstantiateFBX(std::string path)
{
	Ref<FBXResources> res = FBXManager::GetFBX(path);

	return CreateEntityFromFBX(res, nullptr);
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
	transform->SetLocalPosition(res->localPosition);
	transform->SetLocalRotation(res->localRotation);
	transform->SetLocalScale(res->localScale);
	transform->SetParent(parent);

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
