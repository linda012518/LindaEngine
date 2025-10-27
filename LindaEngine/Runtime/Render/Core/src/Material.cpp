#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"
#include "MaterialManager.h"
#include "Mesh.h"

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;
std::string Material::overrideLightMode;

Material::Material()
{
}

Material::~Material()
{
}

void Material::SetShader(const char* path)
{
	_shaderPath = path;
	_passes.clear();
}

bool Material::Bind(Transform* transform, const std::vector<VertexAttribute>& attrubites)
{
	Ref<MaterialPass> pass = nullptr;

	if (_passes.find(overrideLightMode) == _passes.end())
	{
		if (_hasFallback)
		{
			pass = MaterialManager::GetDefaultMaterialPass(overrideLightMode.c_str());
			if (nullptr == pass)
				return false;
			pass->CompileShader(_shaderPath, attrubites);
			pass->Bind(transform);
			return true;
		}
		return false;
	}

	pass = _passes[overrideLightMode];
	pass->CompileShader(_shaderPath, attrubites);
	pass->Bind(transform);
	return true;
}

bool Material::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FilePath" << YAML::Value << _filePath;
	out << YAML::EndMap;

	return true;
}

bool Material::Deserialize(YAML::Node& node)
{
	_filePath = node["FilePath"].as<std::string>();
	return true;
}