#include "MaterialManagerEditor.h"
#include "NodeEditor.h"
#include "MaterialManager.h"
#include "Material.h"
#include "YamlSerializer.h"
#include "Path.h"

#include <sstream>
#include <iostream>
#include <regex>
#include <fstream>

using namespace LindaEditor;
using namespace LindaEngine;

std::vector<Ref<MaterialNodeEditor>> MaterialManagerEditor::_materialNodes;
Ref<MaterialNodeEditor> MaterialManagerEditor::_materialNode;
Ref<ShaderNodeEditor> MaterialManagerEditor::_shaderNode = CreateRef<ShaderNodeEditor>();
std::vector<Ref<ShaderNodeEditor>> MaterialManagerEditor::_shaderNodes;

void MaterialManagerEditor::CreateMaterial(const char* path)
{
	Ref<Material> temp = Material::overrideMat;
	try
	{
		if (nullptr == _shaderNode)
		{
			//TODO 没选中Shader加载默认材质
			return;
		}

		std::string matPath = Path::GetFilePath(_shaderNode->path) + Path::GetFileNameNoExtension(_shaderNode->path) + ".mat";

		_materialNode = CreateRef<MaterialNodeEditor>();
		_materialNode->path = matPath;
		_materialNode->name = Path::GetFileName(matPath);

		Material::overrideMat = MaterialManager::GetMaterialByShader(_shaderNode->path.c_str());
		Material::overrideMat->SetPath(matPath.c_str());

		_materialNodes.push_back(_materialNode);
		SaveMaterial();
		Material::overrideMat = temp;
	}
	catch (const std::exception&)
	{

	}
}

void MaterialManagerEditor::SaveMaterial()
{
	YamlSerializer::SerializeMaterial(_materialNode->path.c_str());
}

void MaterialManagerEditor::Initialize()
{
	//TODO 打开工程，加载所有的材质节点
}

