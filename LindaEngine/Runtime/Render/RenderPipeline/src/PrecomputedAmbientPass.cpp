#include "PrecomputedAmbientPass.h"
#include "Environment.h"
#include "RendererSystem.h"
#include "Light.h"

using namespace LindaEngine;

PrecomputedAmbientPass::PrecomputedAmbientPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRendering;
	Environment::ComputeBRDFLutMap();
}

void PrecomputedAmbientPass::Render(Camera* camera)
{
	if (false == Light::mainLightDirty)
		return;
	Light::mainLightDirty = false;
	Environment::RenderSkyboxMap();
}