#include "DrawErrorPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

DrawErrorPass::DrawErrorPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void DrawErrorPass::Render(Camera* camera)
{
	RendererSystem::DrawErrorRenderer();
}