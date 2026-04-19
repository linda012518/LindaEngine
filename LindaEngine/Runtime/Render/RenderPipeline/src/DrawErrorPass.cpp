#include "DrawErrorPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

DrawErrorPass::DrawErrorPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void DrawErrorPass::Render(Weak<Camera> camera)
{
	RendererSystem::DrawErrorRenderer();
}