#include "SceneManagerEditor.h"
#include "Scene.h"
#include "NodeEditor.h"
#include "Path.h"
#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>

using namespace LindaEditor;
using namespace LindaEngine;

Ref<SceneNodeEditor> SceneManagerEditor::_node = CreateRef<SceneNodeEditor>();
std::vector<Ref<SceneNodeEditor>> SceneManagerEditor::_buildScenes;
std::vector<Ref<SceneNodeEditor>> SceneManagerEditor::_sceneNodes;

bool SceneManagerEditor::SaveScene()
{
	try
	{
		std::string path = "Assets/Scenes/All0.scene";
		if (false == _node->path.empty())
		{
			Path::overridePath = _node->path.c_str();
			return _node->scene->Serialize();
		}

		//TODO 打开保存对话框选择路径
		Path::overridePath = path.c_str();
		bool ret = _node->scene->Serialize();
		if (ret == false)
			return false;

		_node = CreateRef<SceneNodeEditor>();
		_node->path = path;
		_node->name = Path::GetFileName(path);
		_sceneNodes.push_back(_node);
		return true;
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

bool SceneManagerEditor::LoadScene()
{
	YAML::Node data;
	try
	{
		if (nullptr == _node->scene)
		{
			//TODO 加载默认场景
		}
		else
		{
			_node->scene->Destroy();
			data = YAML::LoadFile(_node->path);
			_node->scene->Deserialize(data);
		}
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

Ref<SceneNodeEditor> SceneManagerEditor::GetCurrentNode()
{
	return _node;
}

void SceneManagerEditor::Initialize()
{
	//TODO 打开工程，加载所有的Scene存SceneNodeEditor，并打开对应Scene
}
