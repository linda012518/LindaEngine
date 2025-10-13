#include "Application.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

bool Application::_isQuit = false;

int Application::Initialize()
{
    YamlSerializer::DeSerializeGraphicsConfig("Assets/Config/GraphicsConfig");

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

    return ret;
}

void Application::Finalize()
{
    _graphicContext->Finalize();
    _window->Finalize();
}

void Application::Tick()
{
    while (false == _isQuit)
    {
        _window->Tick();
        _graphicContext->Tick();
    }
}

void Application::Quit()
{
    _isQuit = true;
}
