#include "PrecomputedAmbientPass.h"
#include "Environment.h"
#include "RendererSystem.h"

using namespace LindaEngine;

PrecomputedAmbientPass::PrecomputedAmbientPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRendering;
	Environment::ComputeBRDFLutMap();
}

void PrecomputedAmbientPass::Render(Camera* camera)
{
	// TODO 这里渲染天空盒cubemap，然后生成环境光数据，Skybox Pass 直接采样渲染
	//Environment::RenderSkyboxMap();
}