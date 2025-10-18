#include "MaterialPass.h"
#include "Shader.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Texture.h"

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

void MaterialPass::CompileShader(Ref<ShaderSourceCode> sss)
{
	std::string kw;
	for (auto& keyword : _keywords)
	{
		kw += "#define " + keyword + " \n";
	}
	std::string tempVertex = ShaderManager::defaultShaderVersion + ShaderManager::defaultShaderUniformBlack + kw + sss->vertex;
	std::string tempFragment = ShaderManager::defaultShaderVersion + ShaderManager::defaultShaderUniformBlack + kw + sss->fragment;
	_shader = CreateRef<Shader>(tempVertex.c_str(), tempFragment.c_str());
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
			tud->ID = TextureManager::GetTexture(tud->value)->nativeColorID;
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