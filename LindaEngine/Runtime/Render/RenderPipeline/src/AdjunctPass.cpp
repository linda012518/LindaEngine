#include "AdjunctPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

AdjunctPass::AdjunctPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void AdjunctPass::Render(Weak<Camera> camera)
{
	RendererSystem::DrawAdjunct();
}