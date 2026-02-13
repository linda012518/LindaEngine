#include "UniversalRendererEditor.h"
#include "Settings.h"
#include "SkyboxPass.h"
#include "DrawObjectsPass.h"
#include "PostProcessPasses.h"
#include "AdjunctPass.h"
#include "BlitPickColorPass.h"
#include "DrawErrorPass.h"

using namespace LindaEditor;
using namespace LindaEngine;

void UniversalRendererEditor::CollectRenderPass()
{
    AddRenderPass(CreateRef<SkyboxPass>());
    AddRenderPass(CreateRef<PostProcessPass>());
    AddRenderPass(CreateRef<AdjunctPass>());
    AddRenderPass(CreateRef<DrawErrorPass>());
    AddRenderPass(CreateRef<BlitPickColorPass>());

    DrawingSettings settings;
    settings.lightMode = "Color";

    settings.renderQueueRange = RenderQueueRange::transparent;
    settings.sortSettings.criteria = SortingCriteria::CommonTransparent;
    AddRenderPass(CreateRef<DrawObjectsPass>(RenderPassEvent::BeforeRenderingTransparents, settings));

    settings.renderQueueRange = RenderQueueRange::opaque;
    settings.sortSettings.criteria = SortingCriteria::CommonOpaque;
    AddRenderPass(CreateRef<DrawObjectsPass>(RenderPassEvent::BeforeRenderingOpaques, settings));
}
