#include "Graphic.h"
#include "RenderAPIContext.h"

using namespace LindaEngine;

Scope<RenderAPIContext> Graphic::_renderContext = RenderAPIContext::Create();

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

void Graphic::CreateVertexArrays(int count, unsigned int VAO)
{
	_renderContext->CreateVertexArrays(count, VAO);
}

void Graphic::DeleteVertexArrays(int count, unsigned int VAO)
{
	_renderContext->DeleteVertexArrays(count, VAO);
}

void Graphic::BindVertexArray(unsigned int VAO)
{
	_renderContext->BindVertexArray(VAO);
}

void Graphic::CreateIndexBuffer(unsigned int IBO, int count, void* data, bool is32Bit, bool isStatic)
{
	_renderContext->CreateIndexBuffer(IBO, count, data, is32Bit, isStatic);
}

void Graphic::CreateVertexBuffer(unsigned int VBO, int size, void* data, bool isStatic)
{
	_renderContext->CreateVertexBuffer(VBO, size, data, isStatic);
}

void Graphic::UpdateVertexData(int offset, int size, void* data)
{
	_renderContext->UpdateVertexData(offset, size, data);
}

void Graphic::BindBuffer(unsigned int target, unsigned int buffer)
{
	_renderContext->BindBuffer(target, buffer);
}

void Graphic::UnBindBuffer(unsigned int target)
{
	_renderContext->UnBindBuffer(target);
}

void Graphic::DeleteBuffers(int count, unsigned int buffer)
{
	_renderContext->DeleteBuffers(count, buffer);
}

void Graphic::CheckRenderState(RenderState& state, RenderState& materialState)
{
	_renderContext->CheckRenderState(state, materialState);
}
