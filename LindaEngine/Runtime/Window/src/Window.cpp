#include "Window.h"
#include "WinWindow.h"
#include "GraphicsContext.h"

using namespace LindaEngine;

//实现后期用宏定义割开
Scope<Window> Window::Create()
{
    GraphicsConfig& config = GraphicsContext::graphicsConfig;
    switch (config.platformOS)
    {
    case Platform::Windows:
        return CreateScope<WinWindow>();
        break;
    case Platform::Android:
        break;
    case Platform::Mac:
        break;
    case Platform::IOS:
        break;
    case Platform::Linux:
        break;
    case Platform::Harmony:
        break;
    }
    return nullptr;
}
