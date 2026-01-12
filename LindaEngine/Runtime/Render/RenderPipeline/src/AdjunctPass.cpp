#include "AdjunctPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

AdjunctPass::AdjunctPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRenderingOpaques;
}

void AdjunctPass::Render(Camera* camera)
{
	RendererSystem::DrawAdjunct();
}