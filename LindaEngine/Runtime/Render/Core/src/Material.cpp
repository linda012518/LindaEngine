#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"
#include "MaterialManager.h"
#include "Mesh.h"

#define IMPLEMENT_SETMATERIALUNIFORM(dataType) \
template<> \
void Material::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	for (auto& pass : _passes) \
	{ \
		pass->SetUniformValue<dataType>(name, val, count); \
	} \
}

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;
std::string Material::overrideLightMode = "Color";
bool Material::isPickPass = false;

Material::Material()
{
}

Material::~Material()
{
}

void Material::SetTexture(const std::string& name, std::string value, int pass)
{
	_passes[pass]->SetUniformValue<const char*>(name.c_str(), value.c_str());
}

void Material::SetTexture(const std::string& name, Ref<Texture> value, int pass, int renderTextureColorIndex)
{
	_passes[pass]->SetUniformValue<Ref<Texture>>(name.c_str(), value, renderTextureColorIndex);
}

void Material::SetInt(const std::string& name, int value, int pass)
{
	_passes[pass]->SetUniformValue<int>(name.c_str(), value);
}

void Material::SetFloat(const std::string& name, float value, int pass)
{
	_passes[pass]->SetUniformValue<float>(name.c_str(), value);
}

void Material::SetIntArray(const std::string& name, int count, int* value, int pass)
{
	_passes[pass]->SetUniformValue<int*>(name.c_str(), value, count);
}

void Material::SetFloatArray(const std::string& name, int count, float* value, int pass)
{
	_passes[pass]->SetUniformValue<float*>(name.c_str(), value, count);
}

void Material::SetIVec4(const std::string& name, const glm::ivec4& value, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::ivec4>(name.c_str(), value);
}

void Material::SetVec4(const std::string& name, const glm::vec4& value, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::vec4>(name.c_str(), value);
}

void Material::SetMat2(const std::string& name, const glm::mat2& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat2>(name.c_str(), mat);
}

void Material::SetMat3(const std::string& name, const glm::mat3& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat3>(name.c_str(), mat);
}

void Material::SetMat4(const std::string& name, const glm::mat4& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat4>(name.c_str(), mat);
}

void Material::Bind(Ref<MaterialPass> pass, Transform* transform, const std::vector<VertexAttribute>& attributes)
{
	_hasError = pass->CompileShader(_state.shaderPath, attributes);
	pass->Bind(transform);
}

void Material::Bind(int pass, Transform* transform, const std::vector<VertexAttribute>& attributes)
{
	Bind(_passes[pass], transform, attributes);
}

IMPLEMENT_SETMATERIALUNIFORM(int)
IMPLEMENT_SETMATERIALUNIFORM(float)
IMPLEMENT_SETMATERIALUNIFORM(glm::vec4)
IMPLEMENT_SETMATERIALUNIFORM(glm::ivec4)
IMPLEMENT_SETMATERIALUNIFORM(const char*)

IMPLEMENT_SETMATERIALUNIFORM(glm::mat2)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat3)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat4)

IMPLEMENT_SETMATERIALUNIFORM(int*)
IMPLEMENT_SETMATERIALUNIFORM(float*)
IMPLEMENT_SETMATERIALUNIFORM(glm::vec4*)
IMPLEMENT_SETMATERIALUNIFORM(glm::ivec4*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat2*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat3*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat4*)
IMPLEMENT_SETMATERIALUNIFORM(Ref<Texture>)

void Material::AddKeyword(std::string key)
{
	for (auto& pass : _passes)
	{
		pass->AddKeyword(key);
	}
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

bool Material::CanRender(std::string& lightMode, int minQueue, int maxQueue)
{
	return false == _hasError && _state.renderQueue >= minQueue && _state.renderQueue <= maxQueue && HasLightMode(lightMode);
}

bool Material::HasLightMode(std::string& lightMode)
{
	for (auto& pass : _passes)
	{
		if (pass->_state.lightMode == lightMode)
			return true;
	}

	if (_state.hasFallback)
	{
		if (nullptr != MaterialManager::GetDefaultMaterialPass(lightMode.c_str()))
			return true;
	}
	return false;
}

std::vector<Ref<MaterialPass>> Material::GetPassByLightMode(std::string& lightMode)
{
	std::vector<Ref<MaterialPass>> go;
	for (auto& pass : _passes)
	{
		if (pass->_state.lightMode == lightMode)
			go.push_back(pass);
	}
	if (go.size() <= 0 && _state.hasFallback)
	{
		Ref<MaterialPass> defaultPass = MaterialManager::GetDefaultMaterialPass(lightMode.c_str());
		if (nullptr != defaultPass)
			go.push_back(defaultPass);
	}
	return go;
}
