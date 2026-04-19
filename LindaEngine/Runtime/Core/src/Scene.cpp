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
#include "Environment.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

Weak<Entity> Scene::CreateEntity(const char* name, bool active)
{
	Ref<Entity> e = CreateRef<Entity>(name, active);
	_entitys.push_back(e);
	return e;
}

Weak<Entity> Scene::InstantiateCube(Weak<Transform> parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Cube.FBX", parent);
}

Weak<Entity> Scene::InstantiateSphere(Weak<Transform> parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Sphere.FBX", parent);
}

Weak<Entity> Scene::InstantiatePlane(Weak<Transform> parent)
{
	return InstantiateFBX("BuiltInAssets/Meshs/Plane.FBX", parent);
}

Weak<Entity> Scene::InstantiatePrefab(std::string path, Weak<Transform> parent)
{
	return DeserializePrefab(path, parent);
}

Weak<Entity> Scene::InstantiateFBX(std::string path, Weak<Transform> parent)
{
	Ref<FBXResources> res = FBXManager::GetFBX(path);

	Weak<Entity> entity = CreateEntityFromFBX(res, parent);

	std::vector<Weak<Transform>> transforms;
	std::vector<Weak<SkinMeshRenderer>> renderers;
	CollectBonesComponents(entity, transforms, renderers);

	std::vector<Weak<Transform>> bones;
	for (auto& renderer : renderers)
	{
		bones.clear();

		std::vector<BoneData>& data = renderer->GetBonesData();
		for (auto& boneData : data)
		{
			Weak<Transform> bone = GetTransformByBoneName(transforms, boneData.name);
			bones.push_back(bone);
		}
		renderer->SetBones(bones);
		renderer->SetRootBone(entity->GetTransform());
	}

	return entity;
}

void Scene::DestroyEntity(Weak<Entity> entity)
{
	_dirtyEntitys.push_back(entity);
}

void Scene::DestroyEntityImmediately(Weak<Entity> entity)
{
	Weak<Transform> trans = entity->GetTransform();
	trans->SetParent(nullptr);
	DestroyEntityIncludeChild(entity);
}

Weak<Entity> Scene::GetEntity(const char* name)
{
	for (auto& e : _entitys) {
		if (e->GetName() != name)
			continue;
		return e;
	}
	return nullptr;
}

Weak<Entity> Scene::GetEntity(int id)
{
	for (auto& e : _entitys) {
		if (e->_entityID != id)
			continue;
		return e;
	}
	return nullptr;
}

void Scene::DestroyEntity()
{
	for (auto& e : _dirtyEntitys) 
	{
		Weak<Transform> trans = e->GetTransform();
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

void Scene::DestroyEntityIncludeChild(Weak<Entity> entity)
{
	Weak<Transform> trans = entity->GetTransform();
	const std::list<Weak<Transform>>& children = trans->GetChildren();
	for (auto& t : children)
	{
		Weak<Entity> child = DynamicCastWeak(Entity, t->GetEntity().GetWeak());
		DestroyEntityIncludeChild(child);
	}

	for (auto iter = _entitys.begin(); iter != _entitys.end(); ++iter) {
		if ((*iter)->GetID() != entity->GetID())
			continue;

		entity->Destroy();
		_entitys.erase(iter);
		break;
	}
}

Weak<Entity> Scene::CreateEntityFromFBX(Ref<FBXResources> res, Weak<Transform> parent)
{
	Weak<Entity> entity = CreateEntity(res->name.c_str());

	Weak<Transform> transform = entity->GetTransform();
	transform->SetParent(parent);
	transform->SetLocalPosition(res->localPosition);
	transform->SetLocalRotation(res->localRotation);
	transform->SetLocalScale(res->localScale);

	if (nullptr != res->mesh)
	{
		Weak<Renderer> renderer = nullptr;
		if (res->boneCount > 0)
		{
			Weak<SkinMeshRenderer> skinRenderer = entity->AddComponent<SkinMeshRenderer>();
			skinRenderer->SetBonesData(res->bones);
			renderer = skinRenderer;
		}
		else
			renderer = entity->AddComponent<MeshRenderer>();
		renderer->SetMesh(res->mesh);
		int index = 0;
		for (auto& data : res->mesh->GetAllMeshData())
		{
			renderer->AddMaterial(index++, MaterialManager::GetMaterial("Assets/Materials/test.mat", res->boneCount > 0));
		}
	}
	for (auto& node : res->children)
	{
		CreateEntityFromFBX(node, transform);
	}

	transform->SetParent(parent);
	return entity;
}

void Scene::CollectBonesComponents(Weak<Entity> entity, std::vector<Weak<Transform>>& transforms, std::vector<Weak<SkinMeshRenderer>>& renderers)
{
	Weak<Renderer> renderer = entity->GetComponent<Renderer>();
	if (nullptr == renderer)
	{
		transforms.push_back(entity->GetTransform());
	}
	else
	{
		Weak<SkinMeshRenderer> skinPtr = DynamicCastWeak(SkinMeshRenderer, renderer);
		if (nullptr != skinPtr)
			renderers.push_back(skinPtr);
	}

	for (auto& go : entity->GetTransform()->GetChildren())
	{
		Weak<Entity> child = DynamicCastWeak(Entity, go->GetEntity().GetWeak());
		CollectBonesComponents(child, transforms, renderers);
	}
}

Weak<Transform> Scene::GetTransformByBoneName(std::vector<Weak<Transform>>& transforms, std::string name)
{
	for (auto& t : transforms)
	{
		if (t->GetEntity().GetName() != name)
			continue;
		return t;
	}
	return nullptr;
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
	Environment::SetSkybox(material);
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

void Scene::DuplicateEntity(Weak<Entity> entity)
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

	out << YAML::Key << "SkyboxMaterial" << YAML::Value << Environment::GetSkyboxMaterial()->GetPath();

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

bool Scene::SerializePrefab(std::string path, Weak<Entity> entity)
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

void Scene::SerializeHierarchyPrefab(Weak<Entity> entity)
{
	for (auto& trans : entity->GetTransform()->GetChildren())
	{
		Weak<Entity> go = DynamicCastWeak(Entity, trans->GetEntity().GetWeak());
		go->Serialize();
		SerializeHierarchyPrefab(go);
	}
}

Weak<Entity> Scene::DeserializePrefab(std::string path, Weak<Transform> parent)
{
	YAML::Node data;
	try
	{
		std::vector<Weak<Transform>> list;
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

		Weak<Transform> ret = nullptr;
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
		{
			return DynamicCastWeak(Entity, ret->GetEntity().GetWeak());
		}
		else
			return nullptr;
	}
	catch (const std::exception&)
	{
		std::cout << "Scene::DeserializePrefab Error" << path << "\n" << std::endl;
		return nullptr;
	}

}
