#include "SkyboxPass.h"
#include "RendererSystem.h"
#include "Camera.h"

using namespace LindaEngine;

SkyboxPass::SkyboxPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRenderingSkybox;
}

void SkyboxPass::Render(Camera* camera)
{
	if (camera->GetClearType() == CameraClearType::Skybox)
		RendererSystem::DrawSkybox();
}