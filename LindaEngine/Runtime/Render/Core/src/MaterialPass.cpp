#include "MaterialPass.h"
#include "Transform.h"
#include "Shader.h"
#include "ShaderBuiltInUniform.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Graphic.h"
#include "Application.h"
#include "Entity.h"
#include "MaterialManager.h"

#define IMPLEMENT_SETUNIFORM(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	if (_state.uniformNameMap.find(name) == _state.uniformNameMap.end()) \
		return; \
 \
	DynamicCastRef(UniformClass, _state.uniformNameMap[name])->value = val; \
}

#define IMPLEMENT_SETUNIFORMMATRIX(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	if (_state.uniformNameMap.find(name) != _state.uniformNameMap.end()) \
	{ \
		DynamicCastRef(UniformClass, _state.uniformNameMap[name])->value = val; \
	} \
	else \
	{ \
		Ref<UniformClass> matrix = CreateRef<UniformClass>(); \
		matrix->name = name; \
		matrix->value = val; \
		_state.uniformNameMap[name] = matrix; \
	} \
}

#define IMPLEMENT_SETUNIFORMARRAY(dataType, UniformClass) \
template<> \
void MaterialPass::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	if (_state.uniformNameMap.find(name) != _state.uniformNameMap.end()) \
	{ \
		DynamicCastRef(UniformClass, _state.uniformNameMap[name])->value = val; \
		DynamicCastRef(UniformClass, _state.uniformNameMap[name])->arraySize = count; \
	} \
	else \
	{ \
		Ref<UniformClass> array = CreateRef<UniformClass>(); \
		array->name = name; \
		array->value = val; \
		array->arraySize = count; \
		_state.uniformNameMap[name] = array; \
	} \
}

using namespace LindaEngine;

std::string texelSizeSuffix = "_TexelSize";

RenderState MaterialPass::_defualtState;
RenderState MaterialPass::_currentState;
Ref<MaterialPass> MaterialPass::overrideMatPass = nullptr;

void MaterialPass::AddKeyword(std::string& key)
{
	auto itr = std::find(_state.keywords.begin(), _state.keywords.end(), key);
	if (itr == _state.keywords.end())
		_state.keywords.push_back(key);
}

bool MaterialPass::CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes)
{
	if (nullptr != _shader)
		return _shader->HasError();

	Ref<ShaderSource> ssVector = ShaderManager::GetShaderSource(shaderPath.c_str());

	for (auto& ss : ssVector->shaderSrcCode) {
		if (ss->passState.lightMode != Material::overrideLightMode)
			continue;

		_shader = ShaderManager::CompileShader(ss, _state.keywords, attributes);
		break;
	}

	return _shader->HasError();
}

void MaterialPass::Bind(Transform* transform)
{
	if (nullptr == _shader)
		return;

	LoadTextures();
	
	// 确保renderState已初始化
	if (nullptr == _state.renderState)
		_state.renderState = CreateRef<RenderState>();
	
	Graphic::CheckRenderState(_currentState, *_state.renderState.get());
	_shader->Begin();
	if (nullptr != transform)
	{
		_shader->SetMat4(ShaderBuiltInUniform::linda_LocalToWorld, transform->GetLocalToWorldMat());
		_shader->SetMat4(ShaderBuiltInUniform::linda_WorldToLocal, transform->GetWorldToLocalMat());

		if (Application::module == AppModule::Editor)
		{
			_shader->SetInt(ShaderBuiltInUniform::linda_EntityID, transform->GetEntity().GetEntityID());
		}
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
				std::string texelSize = pair.first + texelSizeSuffix;
				if (_state.uniformNameMap.find(texelSize) != _state.uniformNameMap.end())
				{
					glm::vec4 size;
					size.x = (float)texture->width;
					size.y = (float)texture->height;
					size.z = 1.0f / size.x;
					size.w = 1.0f / size.y;
					_shader->SetVec4(texelSize, size);
				}
			}
			break;
		}
		case UniformType::TEXTUREPOINTER:
		{
			Ref<TexturePointerUniformData> tud = DynamicCastRef(TexturePointerUniformData, pair.second);
			TextureManager::Bind(tud->value, acitveChannel, tud->renderTextureColorIndex);
			_shader->SetInt(pair.first, acitveChannel++);
			std::string texelSize = pair.first + texelSizeSuffix;
			if (_state.uniformNameMap.find(texelSize) != _state.uniformNameMap.end())
			{
				glm::vec4 size;
				size.x = (float)tud->value->width;
				size.y = (float)tud->value->height;
				size.z = 1.0f / size.x;
				size.w = 1.0f / size.y;
				_shader->SetVec4(texelSize, size);
			}
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
		{
			if (pair.first.find(texelSizeSuffix) == std::string::npos)
				_shader->SetVec4(pair.first, DynamicCastRef(Float4UniformData, pair.second)->value);
		}
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
		case UniformType::INT4ARRAY:
		{
			Ref<Int4ArrayUniformData> int4Array = DynamicCastRef(Int4ArrayUniformData, pair.second);
			_shader->SetIVec4Array(pair.first, int4Array->value, int4Array->arraySize);
		}
		break;
		case UniformType::FLOAT4ARRAY:
		{
			Ref<Float4ArrayUniformData> float4Array = DynamicCastRef(Float4ArrayUniformData, pair.second);
			_shader->SetVec4Array(pair.first, float4Array->value, float4Array->arraySize);
		}
		break;
		case UniformType::MAT2:
		{
			Ref<Mat2UniformData> mat2 = DynamicCastRef(Mat2UniformData, pair.second);
			_shader->SetMat2(pair.first, mat2->value);
		}
		break;
		case UniformType::MAT3:
		{
			Ref<Mat3UniformData> mat3 = DynamicCastRef(Mat3UniformData, pair.second);
			_shader->SetMat3(pair.first, mat3->value);
		}
		break;
		case UniformType::MAT4:
		{
			Ref<Mat4UniformData> mat4 = DynamicCastRef(Mat4UniformData, pair.second);
			_shader->SetMat4(pair.first, mat4->value);
		}
		break;
		case UniformType::MAT2ARRAY:
		{
			Ref<Mat2ArrayUniformData> mat2Array = DynamicCastRef(Mat2ArrayUniformData, pair.second);
			_shader->SetMat2Array(pair.first, mat2Array->value, mat2Array->arraySize);
		}
		break;
		case UniformType::MAT3ARRAY:
		{
			Ref<Mat3ArrayUniformData> mat3Array = DynamicCastRef(Mat3ArrayUniformData, pair.second);
			_shader->SetMat3Array(pair.first, mat3Array->value, mat3Array->arraySize);
		}
		break;
		case UniformType::MAT4ARRAY:
		{
			Ref<Mat4ArrayUniformData> mat4Array = DynamicCastRef(Mat4ArrayUniformData, pair.second);
			_shader->SetMat4Array(pair.first, mat4Array->value, mat4Array->arraySize);
		}
		break;
		}

	}
}

void MaterialPass::LoadTextures()
{
	if (true == _textureLoaded)
		return;
	_textureLoaded = true;

	for (const auto& pair : _state.uniformNameMap)
	{
		if (pair.second->dataType != UniformType::TEXTURE)
			continue;
		TextureManager::GetTexture(DynamicCastRef(TextureUniformData, pair.second)->value);
	}
}

IMPLEMENT_SETUNIFORMMATRIX(int, IntUniformData)
IMPLEMENT_SETUNIFORMMATRIX(float, FloatUniformData)
IMPLEMENT_SETUNIFORMMATRIX(glm::vec4, Float4UniformData)
IMPLEMENT_SETUNIFORMMATRIX(glm::ivec4, Int4UniformData)
IMPLEMENT_SETUNIFORMMATRIX(const char*, TextureUniformData)

IMPLEMENT_SETUNIFORMMATRIX(glm::mat2, Mat2UniformData)
IMPLEMENT_SETUNIFORMMATRIX(glm::mat3, Mat3UniformData)
IMPLEMENT_SETUNIFORMMATRIX(glm::mat4, Mat4UniformData)

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

	Ref<RenderTexture> rt = DynamicCastRef(RenderTexture, val);

	if (nullptr == rt && val->isUserCreate == false)
	{
		Ref<TextureUniformData> tud = DynamicCastRef(TextureUniformData, _state.uniformNameMap[name]);
		tud->value = val->nodePath;
	}
	else
	{
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

}

