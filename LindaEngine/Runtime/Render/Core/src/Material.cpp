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

void Material::Bind(Ref<MaterialPass> pass, Transform* transform, const std::vector<VertexAttribute>& attributes)
{
	pass->CompileShader(_state.shaderPath, attributes);
	pass->Bind(transform);
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

std::vector<Ref<MaterialPass>> Material::GetPassByLightMode(std::string& lightMode)
{
	std::vector<Ref<MaterialPass>> go;
	for (auto& pass : _passes)
	{
		if (pass->_state.lightMode != lightMode && _state.hasFallback)
		{
			pass = MaterialManager::GetDefaultMaterialPass(overrideLightMode.c_str());
			if (nullptr == pass)
				continue;
			go.push_back(pass);
		}
		else
			go.push_back(pass);
	}
	return go;
}
