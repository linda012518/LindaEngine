#include "OpenglAPIContext.h"
#include "glad/glad.h"
#include "RenderState.h"

using namespace LindaEngine;

void OpenglAPIContext::Initialize()
{
	glColorMask(true, true, true, true);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLineWidth(1.0f);
	glPointSize(2.0f);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glDisable(GL_BLEND);
	glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenglAPIContext::SetViewport(int xStart, int yStart, int width, int height)
{
	glViewport(xStart, yStart, width, height);
}

void OpenglAPIContext::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void OpenglAPIContext::Clear(bool color, bool depth, bool stencil)
{
	int ret = 0;
	if (color) ret |= GL_COLOR_BUFFER_BIT;
	if (depth) ret |= GL_DEPTH_BUFFER_BIT;
	if (stencil) ret |= GL_STENCIL_BUFFER_BIT;

	glClear(ret);
}

void OpenglAPIContext::CheckRenderState(RenderState& state, RenderState& materialState)
{
	CheckColorMask(state, materialState);
	CheckDepthState(state, materialState);
	CheckCullFaceState(state, materialState);
	CheckPolygonModeState(state, materialState);
	CheckStencilState(state, materialState);
	CheckBlendState(state, materialState);
}

void OpenglAPIContext::CheckColorMask(RenderState& state, RenderState& materialState)
{
	if (state.colorMask == materialState.colorMask)
		return;

	state.colorMask = materialState.colorMask;
	glColorMask(state.colorMask.R, state.colorMask.G, state.colorMask.B, state.colorMask.A);
}

void OpenglAPIContext::CheckDepthState(RenderState& state, RenderState& materialState)
{
	if (state.depthState.depthTest != materialState.depthState.depthTest)
	{
		state.depthState.depthTest = materialState.depthState.depthTest;
		if (state.depthState.depthTest == true)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	if (state.depthState.depthWrite != materialState.depthState.depthWrite)
	{
		state.depthState.depthWrite = materialState.depthState.depthWrite;
		glDepthMask(state.depthState.depthWrite);
	}
	if (state.depthState.depthFunc != materialState.depthState.depthFunc)
	{
		state.depthState.depthFunc = materialState.depthState.depthFunc;
		glDepthFunc(static_cast<int>(state.depthState.depthFunc));
	}
}

void OpenglAPIContext::CheckCullFaceState(RenderState& state, RenderState& materialState)
{
	if (state.cullFaceFunc == materialState.cullFaceFunc)
		return;

	if (materialState.cullFaceFunc == CullFaceFunc::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		if (state.cullFaceFunc == CullFaceFunc::None) glEnable(GL_CULL_FACE);
		glCullFace(static_cast<int>(materialState.cullFaceFunc));
	}
	state.cullFaceFunc = materialState.cullFaceFunc;
}

void OpenglAPIContext::CheckPolygonModeState(RenderState& state, RenderState& materialState)
{
	if (state.polygonModeState.polygonMode != materialState.polygonModeState.polygonMode)
	{
		state.polygonModeState.polygonMode = materialState.polygonModeState.polygonMode;
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<int>(state.polygonModeState.polygonMode));
	}

	if (state.polygonModeState.polygonMode == PolygonMode::FILL)
		return;

	if (state.polygonModeState.polygonMode == PolygonMode::LINE)
	{
		state.polygonModeState.lineWidth = materialState.polygonModeState.lineWidth;
		//state.polygonModeState.smoothLine = _renderState.polygonModeState.smoothLine;
		//if (state.polygonModeState.smoothLine)
		//{
		//	glEnable(GL_LINE_SMOOTH); //∆Ù”√
		//	glHint(GL_LINE_SMOOTH, GL_NICEST);
		//}
		glLineWidth(state.polygonModeState.lineWidth);
	}
	else if (state.polygonModeState.polygonMode == PolygonMode::POINT)
	{
		state.polygonModeState.pointSize = materialState.polygonModeState.pointSize;
		//state.polygonModeState.smoothPoint = _renderState.polygonModeState.smoothPoint;
		//if (state.polygonModeState.smoothPoint)
		//{
		//	glEnable(GL_POINT_SMOOTH); //∆Ù”√
		//	glHint(GL_POINT_SMOOTH, GL_NICEST);

		//}
		glPointSize(state.polygonModeState.pointSize);
	}
}

void OpenglAPIContext::CheckStencilState(RenderState& state, RenderState& materialState)
{
	if (state.stencilState.stencilTest != materialState.stencilState.stencilTest)
	{
		state.stencilState.stencilTest = materialState.stencilState.stencilTest;
		if (true == state.stencilState.stencilTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}

	if (false == state.stencilState.stencilTest)
		return;

	if (state.stencilState.stencilWriteMask != materialState.stencilState.stencilWriteMask)
	{
		state.stencilState.stencilWriteMask = materialState.stencilState.stencilWriteMask;
		glStencilMask(state.stencilState.stencilWriteMask);
	}

	if (state.stencilState.stencilRef != materialState.stencilState.stencilRef ||
		state.stencilState.stencilFuncMask != materialState.stencilState.stencilFuncMask ||
		state.stencilState.stencilFunc != materialState.stencilState.stencilFunc)
	{
		state.stencilState.stencilRef = materialState.stencilState.stencilRef;
		state.stencilState.stencilFuncMask = materialState.stencilState.stencilFuncMask;
		state.stencilState.stencilFunc = materialState.stencilState.stencilFunc;

		glStencilFunc(static_cast<int>(state.stencilState.stencilFunc), state.stencilState.stencilRef, state.stencilState.stencilFuncMask);
	}


	if (state.stencilState.stencilFail != materialState.stencilState.stencilFail ||
		state.stencilState.stencilZFail != materialState.stencilState.stencilZFail ||
		state.stencilState.stencilZPass != materialState.stencilState.stencilZPass)
	{
		state.stencilState.stencilFail = materialState.stencilState.stencilFail;
		state.stencilState.stencilZFail = materialState.stencilState.stencilZFail;
		state.stencilState.stencilZPass = materialState.stencilState.stencilZPass;

		glStencilOp(static_cast<int>(state.stencilState.stencilFail), static_cast<int>(state.stencilState.stencilZFail), static_cast<int>(state.stencilState.stencilZPass));
	}

}

void OpenglAPIContext::CheckBlendState(RenderState& state, RenderState& materialState)
{
	if (state.blendState.enableBlend != materialState.blendState.enableBlend)
	{
		state.blendState.enableBlend = materialState.blendState.enableBlend;
		if (true == state.blendState.enableBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}

	if (false == state.blendState.enableBlend)
		return;

	state.blendState.constantColor = materialState.blendState.constantColor;

	if (state.blendState.blendSrc != materialState.blendState.blendSrc ||
		state.blendState.blendDis != materialState.blendState.blendDis)
	{
		state.blendState.blendSrc = materialState.blendState.blendSrc;
		state.blendState.blendDis = materialState.blendState.blendDis;
	}

	if (state.blendState.blendSrc == BlendFunc::CONSTANT_COLOR || state.blendState.blendSrc == BlendFunc::ONE_MINUS_CONSTANT_COLOR ||
		state.blendState.blendDis == BlendFunc::CONSTANT_COLOR || state.blendState.blendDis == BlendFunc::ONE_MINUS_CONSTANT_COLOR)
		glBlendColor(state.blendState.constantColor.r, state.blendState.constantColor.g, state.blendState.constantColor.b, state.blendState.constantColor.a);

	if (state.blendState.enableBlendSeparate != materialState.blendState.enableBlendSeparate)
		state.blendState.enableBlendSeparate = materialState.blendState.enableBlendSeparate;

	if (false == state.blendState.enableBlendSeparate)
	{
		glBlendFunc(static_cast<int>(state.blendState.blendSrc), static_cast<int>(state.blendState.blendDis));
		return;
	}

	if (state.blendState.blendEqua != materialState.blendState.blendEqua)
	{
		state.blendState.blendEqua = materialState.blendState.blendEqua;
		glBlendEquation(static_cast<int>(state.blendState.blendEqua));
	}
	if (state.blendState.blendSrcAlpha != materialState.blendState.blendSrcAlpha ||
		state.blendState.blendDisAlpha != materialState.blendState.blendDisAlpha)
	{
		state.blendState.blendSrcAlpha = materialState.blendState.blendSrcAlpha;
		state.blendState.blendDisAlpha = materialState.blendState.blendDisAlpha;
	}

	if (state.blendState.blendSrcAlpha == BlendFunc::CONSTANT_ALPHA || state.blendState.blendSrcAlpha == BlendFunc::ONE_MINUS_CONSTANT_ALPHA ||
		state.blendState.blendDisAlpha == BlendFunc::CONSTANT_ALPHA || state.blendState.blendDisAlpha == BlendFunc::ONE_MINUS_CONSTANT_ALPHA)
		glBlendColor(state.blendState.constantColor.r, state.blendState.constantColor.g, state.blendState.constantColor.b, state.blendState.constantColor.a);

	glBlendFuncSeparate(static_cast<int>(state.blendState.blendSrc), static_cast<int>(state.blendState.blendDis), static_cast<int>(state.blendState.blendSrcAlpha), static_cast<int>(state.blendState.blendDisAlpha));


}

