#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"
#include "MaterialManager.h"

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;
std::string Material::overrideLightMode;

Material::Material()
{
}

Material::~Material()
{
}

void Material::CompileShader(Ref<MaterialPass> pass)
{
	if (pass->IsCompiled())
		return;

	Ref<ShaderSource> ssVector = ShaderManager::GetShaderSource(_shaderPath.c_str());

	for (auto& ss : ssVector->shaderSrcCode) {
		if (ss->name != overrideLightMode)
			continue;
		pass->CompileShader(ss);
	}
}

void Material::SetShader(const char* path)
{
	_shaderPath = path;
	_passes.clear();
}

bool Material::Bind()
{
	Ref<MaterialPass> pass = nullptr;

	if (_passes.find(overrideLightMode) == _passes.end())
	{
		if (_hasFallback)
		{
			pass = MaterialManager::GetDefaultMaterialPass(overrideLightMode.c_str());
			if (nullptr == pass)
				return false;

			CompileShader(pass);
			pass->Bind();
			return true;
		}
		return false;
	}

	pass = _passes[overrideLightMode];
	CompileShader(pass);
	pass->Bind();
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