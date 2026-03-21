#include "UniversalRenderer.h"
#include "Settings.h"
#include "SkyboxPass.h"
#include "DrawObjectsPass.h"
#include "PostProcessPasses.h"
#include "PrecomputedAmbientPass.h"

using namespace LindaEngine;

void UniversalRenderer::CollectRenderPass()
{
    AddRenderPass(CreateRef<PrecomputedAmbientPass>());
    AddRenderPass(CreateRef<SkyboxPass>());
    AddRenderPass(CreateRef<PostProcessPass>());

    DrawingSettings settings;
    settings.lightMode = "Color";

    settings.renderQueueRange = RenderQueueRange::transparent;
    settings.sortSettings.criteria = SortingCriteria::CommonTransparent;
    AddRenderPass(CreateRef<DrawObjectsPass>(RenderPassEvent::BeforeRenderingTransparents, settings));

    settings.renderQueueRange = RenderQueueRange::opaque;
    settings.sortSettings.criteria = SortingCriteria::CommonOpaque;
    AddRenderPass(CreateRef<DrawObjectsPass>(RenderPassEvent::BeforeRenderingOpaques, settings));
}
