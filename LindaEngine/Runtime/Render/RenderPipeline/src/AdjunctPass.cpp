#include "AdjunctPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

AdjunctPass::AdjunctPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void AdjunctPass::Render(Camera* camera)
{
	RendererSystem::DrawAdjunct();
}