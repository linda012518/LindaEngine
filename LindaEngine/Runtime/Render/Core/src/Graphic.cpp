#include "Graphic.h"
#include "RenderAPIContext.h"
#include "TextureDriver.h"

using namespace LindaEngine;

Scope<RenderAPIContext> Graphic::_renderContext = nullptr;

void Graphic::Initialize()
{
	_renderContext = RenderAPIContext::Create();
	_renderContext->Initialize();
	TextureDriver::Initialize();
}

void Graphic::SetViewport(int xStart, int yStart, int width, int height)
{
	_renderContext->SetViewport(xStart, yStart, width, height);
}

void Graphic::SetClearColor(float r, float g, float b, float a)
{
	_renderContext->SetClearColor(r, g, b, a);
}

void Graphic::Clear(bool color, bool depth, bool stencil)
{
	_renderContext->Clear(color, depth, stencil);
}

void Graphic::CheckRenderState(RenderState& state, RenderState& materialState)
{
	_renderContext->CheckRenderState(state, materialState);
}
