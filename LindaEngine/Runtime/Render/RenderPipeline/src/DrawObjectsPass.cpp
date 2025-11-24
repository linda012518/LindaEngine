#include "DrawObjectsPass.h"
#include "RendererSystem.h"
#include "Camera.h"

using namespace LindaEngine;

DrawObjectsPass::DrawObjectsPass(RenderPassEvent event, DrawingSettings& settings)
{
	_settings = settings;
	_renderPassEvent = event;
}

void DrawObjectsPass::Render(Camera* camera)
{
	RendererSystem::DrawRenderers(camera, &_settings);
}

