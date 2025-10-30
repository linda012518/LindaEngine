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

bool Material::Bind(Transform* transform, const std::vector<VertexAttribute>& attrubites)
{
	Ref<MaterialPass> pass = nullptr;

	if (_passes.find(overrideLightMode) == _passes.end())
	{
		if (_state.hasFallback)
		{
			pass = MaterialManager::GetDefaultMaterialPass(overrideLightMode.c_str());
			if (nullptr == pass)
				return false;
			pass->CompileShader(_state.shaderPath, attrubites);
			pass->Bind(transform);
			return true;
		}
		return false;
	}

	pass = _passes[overrideLightMode];
	pass->CompileShader(_state.shaderPath, attrubites);
	pass->Bind(transform);
	return true;
}

bool Material::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FilePath" << YAML::Value << _state.materialPath;
	out << YAML::EndMap;

	return true;
}

bool Material::Deserialize(YAML::Node& node)
{
	_state.materialPath = node["FilePath"].as<std::string>();
	return true;
}