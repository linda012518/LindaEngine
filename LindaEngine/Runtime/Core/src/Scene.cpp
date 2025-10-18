#include "Scene.h"
#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "Path.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

Ref<Scene> Scene::overrideScene = nullptr;

Entity* Scene::CreateEntity(const char* name, bool active)
{
	Ref<Entity> e = CreateRef<Entity>(name, active);
	_entitys.push_back(e);
	return e.get();
}

void Scene::DestroyEntity(Entity* entity)
{
	Transform* trans = entity->GetComponent<Transform>();
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

void Scene::DestroyEntityIncludeChild(Entity* entity)
{
	Transform* trans = entity->GetComponent<Transform>();
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

void Scene::Serialize()
{
	YAML::Emitter out;
	YamlSerializer::out = &out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene";
	out << YAML::Value << YAML::BeginMap;
	if (_path == nullptr)
		_path = Path::overridePath;
	out << YAML::Key << "FileName" << YAML::Value << _path;
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
	}
	catch (const std::exception&)
	{
		std::cout << "Scene::Serialize Error" << _path << "\n" << std::endl;
	}

	YamlSerializer::out = nullptr;
}

bool Scene::Deserialize()
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(Path::overridePath);
	}
	catch (const std::exception&)
	{
		return false;
	}

	auto scene = data["Scene"];
	if (!scene)
		return false;

	std::string path = scene["FileName"].as<std::string>();
	_path = path.c_str();

	auto entitys = scene["Entitys"];
	for (auto entity : entitys)
	{
		std::string entityName = entity["Name"].as<std::string>();
		Ref<Entity> e = CreateRef<Entity>(entityName.c_str(), entity["Active"].as<bool>());
		
	}

	return true;

}
