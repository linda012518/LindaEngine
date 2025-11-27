#include "Graphic.h"
#include "RenderAPIContext.h"
#include "TextureDriver.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "GraphicsContext.h"

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

void Graphic::Blit(Ref<RenderTexture> src, Ref<RenderTexture> dest, Ref<Material> mat, int pass)
{
	Ref<RenderTexture> go = nullptr;
	if (src->msaa > 1)
	{
		go = RenderTextureManager::GetBlitRenderTexture(src);
	}
	else
		go = src;

	RenderTextureManager::SetRenderTarget(dest);
	if (nullptr == dest)
	{
		GraphicsConfig& config = GraphicsContext::graphicsConfig;
		SetViewport(0, 0, config.screenNewWidth, config.screenNewHeight);
	}
	else
		SetViewport(0, 0, dest->width, dest->height);
	Clear(true, true, true);
	Material::overrideLightMode = "Color";
	mat->SetTexture("mainTexture", go, pass);
	mat->Bind(pass, nullptr, std::vector<VertexAttribute>());
	MeshManager::GetEmpty()->Draw();
}

void Graphic::Blit(Ref<RenderTexture> src, Ref<RenderTexture> dest)
{
	RenderTextureManager::BlitRenderTexture(src, dest);
}

void Graphic::CheckRenderState(RenderState& state, RenderState& materialState)
{
	_renderContext->CheckRenderState(state, materialState);
}
