#include "SkyboxPass.h"
#include "RendererSystem.h"

using namespace LindaEngine;

SkyboxPass::SkyboxPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRenderingSkybox;
}

void SkyboxPass::Render(Camera* camera)
{
	RendererSystem::DrawSkybox();
}