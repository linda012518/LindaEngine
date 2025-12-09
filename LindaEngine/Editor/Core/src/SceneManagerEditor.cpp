#include "SceneManagerEditor.h"
#include "Scene.h"
#include "Entity.h"
#include "NodeEditor.h"
#include "Path.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "SceneManager.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>

using namespace LindaEditor;
using namespace LindaEngine;

Ref<SceneNodeEditor> SceneManagerEditor::_node;
std::vector<Ref<SceneNodeEditor>> SceneManagerEditor::_buildScenes;
std::vector<Ref<SceneNodeEditor>> SceneManagerEditor::_sceneNodes;
std::string SceneManagerEditor::_defaultScenePath = "BuiltInAssets/Scenes/Default.scene";

bool SceneManagerEditor::SaveScene(std::string path)
{
	try
	{
		if ("" != path)
			_node->scene->SetPath(path);

		return _node->scene->Serialize();
	}
	catch (const std::exception&)
	{
		return false;
	}
}

void SceneManagerEditor::AddToBuild(int index, Ref<SceneNodeEditor> scene)
{
	scene->index = index;

	auto itr = std::find(_buildScenes.begin(), _buildScenes.end(), scene);
	if (itr == _buildScenes.end())
		_buildScenes.push_back(scene);
}

void SceneManagerEditor::RemoveToBuild(Ref<SceneNodeEditor> scene)
{
	auto itr = std::find(_buildScenes.begin(), _buildScenes.end(), scene);
	if (itr != _buildScenes.end())
	{
		scene->index = -1;
		_buildScenes.erase(itr);
	}
}

bool SceneManagerEditor::Build(const char* path)
{
	if (_buildScenes.size() <= 0)
		return false;

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "BuildScenes";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& node : _buildScenes)
	{
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "index" << YAML::Value << node->index;
		out << YAML::Key << "name" << YAML::Value << node->name;
		out << YAML::Key << "path" << YAML::Value << node->path;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(path);
		fout << out.c_str();
		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "YamlSerializer::SerializeGfxConfiguration Error" << path << "\n" << std::endl;
		return false;
	}
}

bool SceneManagerEditor::LoadScene(std::string path)
{
	try
	{
		std::string readPath = path;
		if (path == "")
			readPath = _defaultScenePath;

		_node->path = readPath;
		_node->name = Path::GetFileNameNoExtension(readPath);
		_node->scene = SceneManager::LoadSceneByPath(readPath);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool SceneManagerEditor::IsNewScene()
{
	return _node->path == _defaultScenePath;
}

Ref<SceneNodeEditor> SceneManagerEditor::GetCurrentNode()
{
	return _node;
}

int SceneManagerEditor::Initialize()
{
	_node = CreateRef<SceneNodeEditor>();
	_node->scene = CreateRef<Scene>();
	//TODO 打开工程，加载所有的Scene存SceneNodeEditor，并打开对应Scene

	return 0;
}
