#include "Application.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "Timestamp.h"

using namespace LindaEngine;

bool Application::_isQuit = false;
AppState Application::state = AppState::Loading;
Scope<Window> Application::_window;

int Application::Initialize()
{
    state = AppState::Loading;
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

    return ret;
}

void Application::Finalize()
{
    _graphicContext->Finalize();
    ComponentSystem::Finalize();
    BehaviorSystem::Finalize();
    SceneManager::Finalize();
    _window->Finalize();
}

void Application::Tick()
{
    state = AppState::Running;

    while (false == _isQuit)
    {
        Timestamp::Tick();
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

void Application::Quit()
{
    state = AppState::PrepareQuit;
    _isQuit = true;
}
