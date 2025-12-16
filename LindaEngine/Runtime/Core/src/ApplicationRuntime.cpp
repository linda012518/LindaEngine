#include "ApplicationRuntime.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "Timestamp.h"
#include "LThread.h"
#include "RenderPipeline.h"

using namespace LindaEngine;

int ApplicationRuntime::Initialize()
{
    state = AppState::Loading;
    SetFrameRate(60);

    YamlSerializer::DeSerializeGraphicsConfig(Path::graphicsConfig);

    _window = Window::Create();
    _graphicContext = GraphicsContext::Create(_window.get());

    int ret = 0;

    if ((ret = _window->Initialize()) != 0) {
        printf("Window Initialize Failed...");
        return ret;
    }

    if ((ret = SceneManager::Initialize()) != 0)
    {
        printf("SceneManager Initialize Failed...");
        return ret;
    }

    if ((ret = _graphicContext->Initialize()) != 0) {
        printf("GraphicsContext Initialize Failed...");
        return ret;
    }

    Ref<RenderPipeline> rp = RenderPipeline::Create();
    rp->Initialize();
    _graphicContext->SetRenderPipeline(rp);

    SceneManager::LoadScene(0);

    return ret;
}

void ApplicationRuntime::Finalize()
{
    _graphicContext->Finalize();
    ComponentSystem::Finalize();
    BehaviorSystem::Finalize();
    SceneManager::Finalize();
    _window->Finalize();
}

void ApplicationRuntime::Tick()
{
    Timestamp::Initialize();
    state = AppState::Running;

    double currentInterval = 100.0;

    while (false == _isQuit)
    {
        currentInterval += Timestamp::GetDeltaMilliSecond();
        Timestamp::Tick();
        if (currentInterval < _frameInterval)
        {
            LThread::Sleep(1);
            continue;
        }
        currentInterval = 0.0;

        SceneManager::Tick();
        BehaviorSystem::DoAwake();
        BehaviorSystem::DoOnEnable();
        BehaviorSystem::DoStart();

        BehaviorSystem::DoFixUpdate();
        BehaviorSystem::DoOnTriggerEvent();
        BehaviorSystem::DoOnCollisionEvent();
        _window->Tick();
        BehaviorSystem::DoOnMouseEvent();
        ComponentSystem::Tick();
        BehaviorSystem::DoUpdate();
        BehaviorSystem::DoLateUpdate();

        //BehaviorSystem::OnPreCull();
        BehaviorSystem::DoOnPreRender();
        //BehaviorSystem::OnRenderObject();
        _graphicContext->Tick();
        BehaviorSystem::DoOnPostRender();
        BehaviorSystem::DoOnApplicationPause();
        BehaviorSystem::DoOnApplicationQuit();
        BehaviorSystem::DoOnDisable();
        BehaviorSystem::DoOnDestroy();
        BehaviorSystem::Tick();
    }
}

