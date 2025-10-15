#include "MaterialPass.h"
#include "Shader.h"
#include "TextureManager.h"
#include "ShaderManager.h"

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

void MaterialPass::Setup()
{
	CheckColorMask(_currentState);
	CheckDepthState(_currentState);
	CheckCullFaceState(_currentState);
	CheckPolygonModeState(_currentState);
	CheckStencilState(_currentState);
	CheckBlendState(_currentState);
}

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
			tud->ID = TextureManager::GetTexture(tud->value);
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

void MaterialPass::CheckColorMask(RenderState& state)
{
	if (state.colorMask == _renderState.colorMask)
		return;

	state.colorMask = _renderState.colorMask;
	glColorMask(state.colorMask.R, state.colorMask.G, state.colorMask.B, state.colorMask.A);
}

void MaterialPass::CheckDepthState(RenderState& state)
{
	if (state.depthState.depthTest != _renderState.depthState.depthTest)
	{
		state.depthState.depthTest = _renderState.depthState.depthTest;
		if (state.depthState.depthTest == true)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	if (false == state.depthState.depthTest)
		return;

	if (state.depthState.depthWrite != _renderState.depthState.depthWrite)
	{
		state.depthState.depthWrite = _renderState.depthState.depthWrite;
		glDepthMask(state.depthState.depthWrite);
	}
	if (state.depthState.depthFunc != _renderState.depthState.depthFunc)
	{
		state.depthState.depthFunc = _renderState.depthState.depthFunc;
		glDepthFunc(static_cast<int>(state.depthState.depthFunc));
	}
}

void MaterialPass::CheckCullFaceState(RenderState& state)
{
	if (state.cullFaceFunc == _renderState.cullFaceFunc)
		return;

	if (_renderState.cullFaceFunc == CullFaceFunc::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		if (state.cullFaceFunc == CullFaceFunc::None) glEnable(GL_CULL_FACE);
		glCullFace(static_cast<int>(_renderState.cullFaceFunc));
	}
	state.cullFaceFunc = _renderState.cullFaceFunc;
}

void MaterialPass::CheckPolygonModeState(RenderState& state)
{
	if (state.polygonModeState.polygonMode != _renderState.polygonModeState.polygonMode)
	{
		state.polygonModeState.polygonMode = _renderState.polygonModeState.polygonMode;
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<int>(state.polygonModeState.polygonMode));
	}

	if (state.polygonModeState.polygonMode == PolygonMode::FILL)
		return;

	if (state.polygonModeState.polygonMode == PolygonMode::LINE)
	{
		state.polygonModeState.lineWidth = _renderState.polygonModeState.lineWidth;
		//state.polygonModeState.smoothLine = _renderState.polygonModeState.smoothLine;
		//if (state.polygonModeState.smoothLine)
		//{
		//	glEnable(GL_LINE_SMOOTH); //启用
		//	glHint(GL_LINE_SMOOTH, GL_NICEST);
		//}
		glLineWidth(state.polygonModeState.lineWidth);
	} 
	else if (state.polygonModeState.polygonMode == PolygonMode::POINT)
	{
		state.polygonModeState.pointSize = _renderState.polygonModeState.pointSize;
		//state.polygonModeState.smoothPoint = _renderState.polygonModeState.smoothPoint;
		//if (state.polygonModeState.smoothPoint)
		//{
		//	glEnable(GL_POINT_SMOOTH); //启用
		//	glHint(GL_POINT_SMOOTH, GL_NICEST);

		//}
		glPointSize(state.polygonModeState.pointSize);
	}
}

void MaterialPass::CheckStencilState(RenderState& state)
{
	if (state.stencilState.stencilTest != _renderState.stencilState.stencilTest)
	{
		state.stencilState.stencilTest = _renderState.stencilState.stencilTest;
		if (true == state.stencilState.stencilTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}

	if (false == state.stencilState.stencilTest)
		return;

	if (state.stencilState.stencilWriteMask != _renderState.stencilState.stencilWriteMask)
	{
		state.stencilState.stencilWriteMask = _renderState.stencilState.stencilWriteMask;
		glStencilMask(state.stencilState.stencilWriteMask);
	}

	if (state.stencilState.stencilRef != _renderState.stencilState.stencilRef ||
		state.stencilState.stencilFuncMask != _renderState.stencilState.stencilFuncMask ||
		state.stencilState.stencilFunc != _renderState.stencilState.stencilFunc)
	{
		state.stencilState.stencilRef = _renderState.stencilState.stencilRef;
		state.stencilState.stencilFuncMask = _renderState.stencilState.stencilFuncMask;
		state.stencilState.stencilFunc = _renderState.stencilState.stencilFunc;

		glStencilFunc(static_cast<int>(state.stencilState.stencilFunc), state.stencilState.stencilRef, state.stencilState.stencilFuncMask);
	}


	if (state.stencilState.stencilFail != _renderState.stencilState.stencilFail ||
		state.stencilState.stencilZFail != _renderState.stencilState.stencilZFail ||
		state.stencilState.stencilZPass != _renderState.stencilState.stencilZPass)
	{
		state.stencilState.stencilFail = _renderState.stencilState.stencilFail;
		state.stencilState.stencilZFail = _renderState.stencilState.stencilZFail;
		state.stencilState.stencilZPass = _renderState.stencilState.stencilZPass;

		glStencilOp(static_cast<int>(state.stencilState.stencilFail), static_cast<int>(state.stencilState.stencilZFail), static_cast<int>(state.stencilState.stencilZPass));
	}
}

void MaterialPass::CheckBlendState(RenderState& state)
{
	if (state.blendState.enableBlend != _renderState.blendState.enableBlend)
	{
		state.blendState.enableBlend = _renderState.blendState.enableBlend;
		if (true == state.blendState.enableBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
	
	if (false == state.blendState.enableBlend)
		return;

	state.blendState.constantColor = _renderState.blendState.constantColor;

	if (state.blendState.blendSrc != _renderState.blendState.blendSrc ||
		state.blendState.blendDis != _renderState.blendState.blendDis)
	{
		state.blendState.blendSrc = _renderState.blendState.blendSrc;
		state.blendState.blendDis = _renderState.blendState.blendDis;
	}

	if (state.blendState.blendSrc == BlendFunc::CONSTANT_COLOR || state.blendState.blendSrc == BlendFunc::ONE_MINUS_CONSTANT_COLOR ||
		state.blendState.blendDis == BlendFunc::CONSTANT_COLOR || state.blendState.blendDis == BlendFunc::ONE_MINUS_CONSTANT_COLOR)
		glBlendColor(state.blendState.constantColor.r, state.blendState.constantColor.g, state.blendState.constantColor.b, state.blendState.constantColor.a);

	if (state.blendState.enableBlendSeparate != _renderState.blendState.enableBlendSeparate)
		state.blendState.enableBlendSeparate = _renderState.blendState.enableBlendSeparate;

	if (false == state.blendState.enableBlendSeparate)
	{
		glBlendFunc(static_cast<int>(state.blendState.blendSrc), static_cast<int>(state.blendState.blendDis));
		return;
	}

	if (state.blendState.blendEqua != _renderState.blendState.blendEqua)
	{
		state.blendState.blendEqua = _renderState.blendState.blendEqua;
		glBlendEquation(static_cast<int>(state.blendState.blendEqua));
	}
	if (state.blendState.blendSrcAlpha != _renderState.blendState.blendSrcAlpha ||
		state.blendState.blendDisAlpha != _renderState.blendState.blendDisAlpha)
	{
		state.blendState.blendSrcAlpha = _renderState.blendState.blendSrcAlpha;
		state.blendState.blendDisAlpha = _renderState.blendState.blendDisAlpha;
	}

	if (state.blendState.blendSrcAlpha == BlendFunc::CONSTANT_ALPHA || state.blendState.blendSrcAlpha == BlendFunc::ONE_MINUS_CONSTANT_ALPHA ||
		state.blendState.blendDisAlpha == BlendFunc::CONSTANT_ALPHA || state.blendState.blendDisAlpha == BlendFunc::ONE_MINUS_CONSTANT_ALPHA)
		glBlendColor(state.blendState.constantColor.r, state.blendState.constantColor.g, state.blendState.constantColor.b, state.blendState.constantColor.a);

	glBlendFuncSeparate(static_cast<int>(state.blendState.blendSrc), static_cast<int>(state.blendState.blendDis), static_cast<int>(state.blendState.blendSrcAlpha), static_cast<int>(state.blendState.blendDisAlpha));

}
