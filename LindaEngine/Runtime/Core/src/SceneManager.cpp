#include "SceneManager.h"
#include "Scene.h"
#include "Path.h"

#include "EventSystem.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "TextureManager.h"
#include "FBXManager.h"
#include "MaterialManager.h"
#include "ShaderManager.h"

#include <yaml-cpp/yaml.h>

using namespace LindaEngine;

bool SceneManager::_loadFinish = false;
Ref<Scene> SceneManager::_currentScene = CreateRef<Scene>();
std::unordered_map<int, std::string> SceneManager::_sceneIndexMap;
std::unordered_map<std::string, std::string> SceneManager::_sceneNameMap;

int SceneManager::Initialize()
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(Path::sceneConfig);

		data = data["BuildScenes"];
		if (!data)
			return -1;

		for (auto scene : data)
		{
			int index = scene["index"].as<int>();
			std::string name = scene["name"].as<std::string>();
			std::string path = scene["path"].as<std::string>();

			_sceneIndexMap[index] = path;
			_sceneNameMap[name] = path;
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

void SceneManager::Finalize()
{
	_currentScene->Destroy();
	_currentScene = nullptr;
	_sceneIndexMap.clear();
	_sceneNameMap.clear();
}

void SceneManager::Tick()
{
	//TODO 异步加载和切换场景？？
	_currentScene->UpdateEntityComponents();
	_currentScene->DestroyEntity();
}

Ref<Scene> SceneManager::LoadScene(const char* name)
{
	if (_sceneNameMap.find(name) == _sceneNameMap.end())
		return _currentScene;

	return LoadSceneFromPath(_sceneNameMap[name]);
}

Ref<Scene> SceneManager::LoadScene(int index)
{
	if (_sceneIndexMap.find(index) == _sceneIndexMap.end())
		return _currentScene;

	return LoadSceneFromPath(_sceneIndexMap[index]);
}

Ref<Scene> SceneManager::LoadSceneByPath(std::string path)
{
	return LoadSceneFromPath(path);
}

Ref<Scene> SceneManager::LoadSceneFromPath(std::string& path)
{
	EventSystem::Clear();
	ComponentSystem::Finalize();
	BehaviorSystem::Finalize();
	TextureManager::Clear();
	RenderTextureManager::Clear();
	MaterialManager::Clear();
	ShaderManager::Clear();
	FBXManager::Clear();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
		_currentScene->Destroy();
		_currentScene->Deserialize(data);

		ComponentSystem::OnDeserializeFinish();
		BehaviorSystem::OnDeserializeFinish();

		_currentScene->ResetSceneUUID();
	}
	catch (const std::exception&)
	{
		std::cout << "SceneManager::LoadSceneFromPath Error" << path << "\n" << std::endl;
	}
	return _currentScene;
}

