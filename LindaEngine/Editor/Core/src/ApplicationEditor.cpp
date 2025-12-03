#include "ApplicationEditor.h"
#include "YamlSerializer.h"
#include "Path.h"
#include "Timestamp.h"
#include "LThread.h"
#include "RenderPipeline.h"

using namespace LindaEditor;
using namespace LindaEngine;

int ApplicationEditor::Initialize()
{
    state = AppState::Editor;

    YamlSerializer::DeSerializeGraphicsConfig(Path::graphicsConfig);

    _window = Window::Create();
    _graphicContext = GraphicsContext::Create(_window.get());

    int ret = 0;

    if ((ret = _window->Initialize()) != 0) {
        printf("Window Initialize Failed...");
        return ret;
    }

    if ((ret = _graphicContext->Initialize()) != 0) {
        printf("GraphicsContext Initialize Failed...");
        return ret;
    }

    _graphicContext->SetRenderPipeline(RenderPipeline::Create());

    return ret;
}

void ApplicationEditor::Finalize()
{
    _graphicContext->Finalize();
    _window->Finalize();
}

void ApplicationEditor::Tick()
{
    while (false == _isQuit)
    {
        _window->Tick();
        _graphicContext->Tick();
    }
}

