#include "MaterialPass.h"
#include "Transform.h"
#include "Shader.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Graphic.h"

#define IMPLEMENT_SETUNIFORM(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val) \
{ \
	if (_state.uniformNameMap.find(name) == _state.uniformNameMap.end()) \
		return; \
 \
	DynamicCastRef(UniformClass, _state.uniformNameMap[name])->value = val; \
}

using namespace LindaEngine;

RenderState MaterialPass::_defualtState;
RenderState MaterialPass::_currentState;
Ref<MaterialPass> MaterialPass::overrideMatPass = nullptr;

void MaterialPass::AddKeyword(std::string& key)
{
	auto itr = std::find(_state.keywords.begin(), _state.keywords.end(), key);
	if (itr == _state.keywords.end())
		_state.keywords.push_back(key);
}

void MaterialPass::CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes)
{
	if (nullptr != _shader)
		return;

	Ref<ShaderSource> ssVector = ShaderManager::GetShaderSource(shaderPath.c_str());

	for (auto& ss : ssVector->shaderSrcCode) {
		if (ss->passState.lightMode != Material::overrideLightMode)
			continue;

		_shader = ShaderManager::CompileShader(ss, _state.keywords, attributes);
		break;
	}
}

void MaterialPass::Bind(Transform* transform)
{
	Graphic::CheckRenderState(_currentState, *_state.renderState.get());
	_shader->Begin();
	_shader->SetMat4("_localToWorld", transform->GetLocalToWorldMat());
	_shader->SetMat4("_worldToLocal", transform->GetWorldToLocalMat());
	UpdateUniforms();
}

void MaterialPass::UpdateUniforms()
{
	int acitveChannel = 0;

	for (const auto& pair : _state.uniformNameMap) {

		switch (pair.second->dataType)
		{
		case UniformType::TEXTURE:
		{
			Ref<TextureUniformData> tud = DynamicCastRef(TextureUniformData, pair.second);
			Ref<Texture> texture = TextureManager::GetTexture(tud->value);
			if (nullptr != texture)
			{
				TextureManager::Bind(texture, acitveChannel);
				_shader->SetInt(pair.first, acitveChannel++);
			}
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