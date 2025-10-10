#include "Renderer.h"

using namespace LindaEngine;

Renderer::Renderer(Entity& entity, bool enable) : Component(entity, enable)
{
}

Renderer::~Renderer()
{
}

MeshRenderer::MeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
}

MeshRenderer::~MeshRenderer()
{
}

SkinMeshRenderer::SkinMeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
}

SkinMeshRenderer::~SkinMeshRenderer()
{
}
