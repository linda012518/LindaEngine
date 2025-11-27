#include "UniversalRenderer.h"
#include "Camera.h"
#include "Settings.h"
#include "SkyboxPass.h"
#include "DrawObjectsPass.h"
#include "PostProcessPasses.h"

#include <algorithm>

using namespace LindaEngine;

void UniversalRenderer::Initialize()
{
    CollectRenderPass();
    for (auto& pass : _renderPasses)
    {
        pass->Initialize();
    }
}

void UniversalRenderer::Finalize()
{
    for (auto& pass : _renderPasses)
    {
        pass->Finalize();
    }
    _renderPasses.clear();
}

void UniversalRenderer::Render(Camera* camera)
{
    for (auto& pass : _renderPasses)
    {
        pass->Render(camera);
    }
}

void UniversalRenderer::AddRenderPass(Ref<ScriptablePass> pass)
{
    _renderPasses.push_back(pass);

    std::sort(_renderPasses.begin(), _renderPasses.end(), [](const Ref<ScriptablePass> a, const Ref<ScriptablePass> b) { return a->GetRenderPassEvent() < b->GetRenderPassEvent(); });
}

void UniversalRenderer::CollectRenderPass()
{
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
