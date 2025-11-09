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
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	if (_state.uniformNameMap.find(name) == _state.uniformNameMap.end()) \
		return; \
 \
	DynamicCastRef(UniformClass, _state.uniformNameMap[name])->value = val; \
}

#define IMPLEMENT_SETUNIFORMARRAY(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	if (_state.uniformNameMap.find(name) == _state.uniformNameMap.end()) \
		return; \
	Ref<UniformClass> data = DynamicCastRef(UniformClass, _state.uniformNameMap[name]); \
	data->value = val; \
	data->arraySize = count; \
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
	if (nullptr == _shader)
		return;
	
	// 确保renderState已初始化
	if (nullptr == _state.renderState)
		_state.renderState = CreateRef<RenderState>();
	
	Graphic::CheckRenderState(_currentState, *_state.renderState.get());
	_shader->Begin();
	if (nullptr != transform)
	{
		_shader->SetMat4("_localToWorld", transform->GetLocalToWorldMat());
		_shader->SetMat4("_worldToLocal", transform->GetWorldToLocalMat());
	}
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
		case UniformType::TEXTUREPOINTER:
		{
			Ref<TexturePointerUniformData> tud = DynamicCastRef(TexturePointerUniformData, pair.second);
			Ref<Texture> texture = tud->value;
			if (nullptr == texture)
				break;

			if (texture->type == TextureType::RenderTexture)
				TextureManager::Bind(texture, acitveChannel, tud->renderTextureColorIndex);
			else if (texture->type == TextureType::Tex2D)
				TextureManager::Bind(texture, acitveChannel);

			_shader->SetInt(pair.first, acitveChannel++);
		}
		break;
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

		case UniformType::INTARRAY:
		{
			Ref<IntArrayUniformData> intArray = DynamicCastRef(IntArrayUniformData, pair.second);
			_shader->SetIntArray(pair.first, intArray->arraySize, intArray->value);
		}
		break;
		case UniformType::FLOATARRAY:
		{
			Ref<FloatArrayUniformData> floatArray = DynamicCastRef(FloatArrayUniformData, pair.second);
			_shader->SetFloatArray(pair.first, floatArray->arraySize, floatArray->value);
		}
		break;
		}

	}
}

IMPLEMENT_SETUNIFORM(int, IntUniformData)
IMPLEMENT_SETUNIFORM(float, FloatUniformData)
IMPLEMENT_SETUNIFORM(glm::vec4, Float4UniformData)
IMPLEMENT_SETUNIFORM(glm::ivec4, Int4UniformData)
IMPLEMENT_SETUNIFORM(const char*, TextureUniformData)

IMPLEMENT_SETUNIFORM(glm::mat2, Mat2UniformData)
IMPLEMENT_SETUNIFORM(glm::mat3, Mat3UniformData)
IMPLEMENT_SETUNIFORM(glm::mat4, Mat4UniformData)

IMPLEMENT_SETUNIFORMARRAY(int*, IntArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(float*, FloatArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(glm::vec4*, Float4ArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(glm::ivec4*, Int4ArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(glm::mat2*, Mat2ArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(glm::mat3*, Mat3ArrayUniformData)
IMPLEMENT_SETUNIFORMARRAY(glm::mat4*, Mat4ArrayUniformData)

template<>
void MaterialPass::SetUniformValue<Ref<Texture>>(const char* name, Ref<Texture> val, int count)
{
	if (_state.uniformNameMap.find(name) == _state.uniformNameMap.end())
		return;
	Ref<TexturePointerUniformData> data = DynamicCastRef(TexturePointerUniformData, _state.uniformNameMap[name]);
	if (nullptr == data)
	{
		data = CreateRef<TexturePointerUniformData>();
		data->name = name;
		_state.uniformNameMap[name] = data;
	}

	data->value = val;
	data->renderTextureColorIndex = count;
}

