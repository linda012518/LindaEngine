#include "Application.h"
#include "YamlSerializer.h"
#include "Path.h"

using namespace LindaEngine;

bool Application::_isQuit = false;

int Application::Initialize()
{
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

    return ret;
}

void Application::Finalize()
{
    _graphicContext->Finalize();
    SceneManager::Finalize();
    _window->Finalize();
}

void Application::Tick()
{
    while (false == _isQuit)
    {
        _window->Tick();
        SceneManager::Tick();
        _graphicContext->Tick();
    }
}

void Application::Quit()
{
    _isQuit = true;
}
