#include "ScriptableRenderer.h"
#include "Camera.h"

#include <algorithm>

using namespace LindaEngine;

void ScriptableRenderer::Initialize()
{
    CollectRenderPass();
    for (auto& pass : _renderPasses)
    {
        pass->Initialize();
    }
}

void ScriptableRenderer::Finalize()
{
    for (auto& pass : _renderPasses)
    {
        pass->Finalize();
    }
    _renderPasses.clear();
}

void ScriptableRenderer::Render(Camera* camera)
{
    for (auto& pass : _renderPasses)
    {
        pass->Render(camera);
    }
}

void ScriptableRenderer::AddRenderPass(Ref<ScriptablePass> pass)
{
    _renderPasses.push_back(pass);

    std::sort(_renderPasses.begin(), _renderPasses.end(), [](const Ref<ScriptablePass> a, const Ref<ScriptablePass> b) { return a->GetRenderPassEvent() < b->GetRenderPassEvent(); });
}
