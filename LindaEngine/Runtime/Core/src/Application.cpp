#include "Application.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "Timestamp.h"
#include "LThread.h"

using namespace LindaEngine;

bool Application::_isQuit = false;
AppState Application::state = AppState::Loading;
Scope<Window> Application::_window;

int Application::Initialize()
{
    state = AppState::Loading;
    SetFrameRate(60);

    YamlSerializer::DeSerializeGraphicsConfig(Path::graphicsConfig);

    _window = Window::Create();
    _graphicContext = GraphicsContext::Create(_window.get());

    int ret = 0;

    Timestamp::Initialize();

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

    SceneManager::LoadScene(0);
    ComponentSystem::OnDeserializeFinish();
    BehaviorSystem::OnDeserializeFinish();

    _imgui.Initialize();

    return ret;
}

void Application::Finalize()
{
    _imgui.Finalize();

    _graphicContext->Finalize();
    ComponentSystem::Finalize();
    BehaviorSystem::Finalize();
    SceneManager::Finalize();
    _window->Finalize();
}

void Application::Tick()
{
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

        _imgui.Begin();
        _imgui.OnImGuiRender();
        _imgui.End();

        _graphicContext->SwapBuffers();
    }
}

void Application::SetFrameRate(int rate)
{
    _frameRate = rate;
    _frameInterval = 1.0f / _frameRate;
    _frameInterval *= 1000;
}

void Application::Quit()
{
    state = AppState::PrepareQuit;
    _isQuit = true;
}
