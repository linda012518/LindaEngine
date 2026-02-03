#include "ApplicationEditor.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "Timestamp.h"
#include "LThread.h"
#include "RenderPipeline.h"
#include "SceneManagerEditor.h"
#include "SceneManager.h"

using namespace LindaEditor;
using namespace LindaEngine;

int ApplicationEditor::Initialize()
{
    state = AppState::Loading;

    YamlSerializer::DeSerializeGraphicsConfig(Path::graphicsConfig);

    _window = Window::Create();
    _graphicContext = GraphicsContext::Create(_window.get());

    int ret = 0;

    if ((ret = _window->Initialize()) != 0) {
        printf("Window Initialize Failed...");
        return ret;
    }

    if ((ret = SceneManagerEditor::Initialize()) != 0)
    {
        printf("SceneManagerEditor Initialize Failed...");
        return ret;
    }

    if ((ret = _graphicContext->Initialize()) != 0) {
        printf("GraphicsContext Initialize Failed...");
        return ret;
    }

    module = AppModule::Runtime;
    _runtimeRP = RenderPipeline::Create();
    _runtimeRP->Initialize();

    module = AppModule::Editor;
    _editorRP = RenderPipeline::Create();
    _editorRP->Initialize();

    _imgui.Initialize();

    SceneManagerEditor::LoadScene("BuiltInAssets/Scenes/Default.scene");

    return ret;
}

void ApplicationEditor::Finalize()
{
    _imgui.Finalize();
    _graphicContext->Finalize();
    ComponentSystem::Finalize();
    BehaviorSystem::Finalize();
    SceneManager::Finalize();
    _window->Finalize();
}

void ApplicationEditor::Tick()
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

        _imgui.Begin();
        _imgui.OnImGuiRender();

        _imgui.SetEditRenderTexture();
        _editorRP->Tick();
        _imgui.SetPlayRenderTexture();
        _runtimeRP->Tick();

        _imgui.End();


        _graphicContext->Tick();

        BehaviorSystem::DoOnPostRender();
        BehaviorSystem::DoOnApplicationPause();
        BehaviorSystem::DoOnApplicationQuit();
        BehaviorSystem::DoOnDisable();
        BehaviorSystem::Tick();
    }
}

