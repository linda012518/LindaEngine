#include "ApplicationEditor.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "Timestamp.h"
#include "LThread.h"
#include "RenderPipeline.h"
#include "SceneManagerEditor.h"

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

    module = AppModule::Editor;
    _editorRP = RenderPipeline::Create();
    _editorRP->Initialize();

    module = AppModule::Runtime;
    _runtimeRP = RenderPipeline::Create();
    _runtimeRP->Initialize();

    _imgui.Initialize();

    return ret;
}

void ApplicationEditor::Finalize()
{
    _imgui.Finalize();
    _graphicContext->Finalize();
    _window->Finalize();
}

void ApplicationEditor::Tick()
{
    while (false == _isQuit)
    {
        _window->Tick();

        _imgui.SetEditRenderTexture();
        _editorRP->Tick();
        _imgui.SetPlayRenderTexture();
        _runtimeRP->Tick();

        _imgui.Begin();
        _imgui.OnImGuiRender();
        _imgui.End();

        _graphicContext->Tick();
    }
}

