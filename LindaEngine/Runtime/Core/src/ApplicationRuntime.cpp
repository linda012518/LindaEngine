#include "ApplicationRuntime.h"
#include "RenderPipeline.h"

using namespace LindaEngine;

int ApplicationRuntime::Initialize()
{
    int ret = Application::Initialize();
    if (ret != 0)
        return ret;

    Ref<RenderPipeline> rp = RenderPipeline::Create();
    rp->Initialize();
    _graphicContext->SetRenderPipeline(rp);

    SceneManager::LoadScene(0);

    return ret;
}

void ApplicationRuntime::Finalize()
{
    Application::Finalize();
}

