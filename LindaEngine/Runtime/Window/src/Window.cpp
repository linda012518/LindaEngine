#include "Window.h"
#include "WinWindow.h"

using namespace LindaEngine;

//实现后期用宏定义割开
Scope<Window> Window::Create(Platform platform, GfxConfiguration& gfx)
{
    switch (platform)
    {
    case Platform::Windows:
        return CreateScope<WinWindow>(gfx);
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
