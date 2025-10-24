#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;
Ref<MaterialPass> Material::defaultDepthPass = nullptr;
Ref<MaterialPass> Material::defaultDepthNormalPass = nullptr;
Ref<MaterialPass> Material::defaultShadowCasterPass = nullptr;

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
		if (ss->name == "ShadowCaster")
		{
			if (nullptr == _shadowCasterPass)
				_shadowCasterPass = CreateRef<MaterialPass>();
			_shadowCasterPass->CompileShader(ss);
		}
		else if (ss->name == "Depth")
		{
			if (nullptr == _depthPass)
				_depthPass = CreateRef<MaterialPass>();
			_depthPass->CompileShader(ss);
		}
		else if (ss->name == "DepthNormal")
		{
			if (nullptr == _depthNormalPass)
				_depthNormalPass = CreateRef<MaterialPass>();
			_depthNormalPass->CompileShader(ss);
		}
		else
		{
			if (_colorPasses.size() == 0)
			{
				Ref<MaterialPass> pass = CreateRef<MaterialPass>();
				pass->CompileShader(ss);
				_colorPasses.push_back(pass);
			}
			else
			{
				for (auto& pass : _colorPasses) {
					if (ss->name != pass->_lightMode)
						continue;
					pass->CompileShader(ss);
				}
			}

		}
	}
}

void Material::SetShader(const char* path)
{
	_shaderPath = path;
	_depthPass = nullptr;
	_shadowCasterPass = nullptr;
	_depthNormalPass = nullptr;
	_colorPasses.clear();
}

bool Material::Bind(std::string& lightMode)
{
	if (_passes.find(lightMode) == _passes.end())
		return false;

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