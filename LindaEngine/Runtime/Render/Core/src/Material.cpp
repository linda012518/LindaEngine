#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"
#include "MaterialManager.h"

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;

Material::Material()
{
}

Material::~Material()
{
}

void Material::CompileShader()
{
	Ref<ShaderSource> ssVector = ShaderManager::GetShaderSource(_shaderPath.c_str());
	_hasFallback = ssVector->hasFallback;

	for (auto& ss : ssVector->shaderSrcCode) {
		//Ref<MaterialPass> pass = CreateRef<MaterialPass>();
		//pass->CompileShader(ss);
		//_colorPasses.push_back(pass);
	}
}

void Material::SetShader(const char* path)
{
	_shaderPath = path;
	_passes.clear();
}

bool Material::Bind(std::string& lightMode)
{
	if (_passes.find(lightMode) == _passes.end())
	{
		if (_hasFallback)
		{
			Ref<MaterialPass> pass = MaterialManager::GetDefaultMaterialPass(lightMode.c_str());
			if (nullptr == pass)
				return false;
			pass->Bind();
			return true;
		}
		return false;
	}

	_passes[lightMode]->Bind();
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