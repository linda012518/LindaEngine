#include "MaterialPass.h"
#include "Transform.h"
#include "Shader.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Mesh.h"

#define IMPLEMENT_SETUNIFORM(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val) \
{ \
	if (_uniformNameMap.find(name) == _uniformNameMap.end()) \
		return; \
 \
	DynamicCastRef(UniformClass, _uniformNameMap[name])->value = val; \
}

using namespace LindaEngine;

RenderState MaterialPass::_defualtState;
RenderState MaterialPass::_currentState;
Ref<MaterialPass> MaterialPass::overrideMatPass = nullptr;

void MaterialPass::AddKeyword(std::string& key)
{
	auto itr = std::find(_keywords.begin(), _keywords.end(), key);
	if (itr == _keywords.end())
		_keywords.push_back(key);
}

void MaterialPass::CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes)
{
	if (nullptr != _shader)
		return;

	Ref<ShaderSource> ssVector = ShaderManager::GetShaderSource(shaderPath.c_str());

	for (auto& ss : ssVector->shaderSrcCode) {
		if (ss->name != Material::overrideLightMode)
			continue;

		_shader = ShaderManager::CompileShader(ss, _keywords, attributes);
		break;
	}
}

void MaterialPass::Bind(Transform* transform)
{
	_shader->Begin();
	_shader->SetMat4("_localToWorld", transform->GetLocalToWorldMat());
	_shader->SetMat4("_worldToLocal", transform->GetWorldToLocalMat());
	//UpdateUniforms();
}

void MaterialPass::UpdateUniforms()
{
	_acitveChannel = 0;

	for (const auto& pair : _uniformNameMap) {

		switch (pair.second->dataType)
		{
		case UniformType::TEXTURE:
		{
			Ref<TextureUniformData> tud = DynamicCastRef(TextureUniformData, pair.second);
			tud->ID = TextureManager::GetTexture(tud->value.c_str())->nativeColorID;
			//TODO 需要Texture类 纹理过滤等等信息
			tud->acitveChannel = _acitveChannel++;
			_shader->SetInt(pair.first, tud->ID);
			break;
		}
		case UniformType::INT:
			_shader->SetInt(pair.first, DynamicCastRef(IntUniformData, pair.second)->value);
			break;
		case UniformType::INT4:
			_shader->SetIVec4(pair.first, DynamicCastRef(Int4UniformData, pair.second)->value);
			break;
		case UniformType::FLOAT:
			_shader->SetFloat(pair.first, DynamicCastRef(FloatUniformData, pair.second)->value);
			break;
		case UniformType::FLOAT4:
			_shader->SetVec4(pair.first, DynamicCastRef(Float4UniformData, pair.second)->value);
			break;
		}

	}
}

IMPLEMENT_SETUNIFORM(int, IntUniformData)
IMPLEMENT_SETUNIFORM(float, FloatUniformData)
IMPLEMENT_SETUNIFORM(glm::vec4, Float4UniformData)
IMPLEMENT_SETUNIFORM(glm::ivec4, Int4UniformData)
IMPLEMENT_SETUNIFORM(const char*, TextureUniformData)