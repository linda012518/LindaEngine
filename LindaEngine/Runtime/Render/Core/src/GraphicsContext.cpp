#include "GraphicsContext.h"

#include "WinWindow.h"
#include "OpenglGraphicsContext.h"

using namespace LindaEngine;

//实现后期用宏定义割开
Scope<GraphicsContext> GraphicsContext::Create(GraphicsDriverAPI api, void* window)
{
    switch (api)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateScope<OpenglGraphicsContext>(static_cast<WinWindow*>(window));
    case GraphicsDriverAPI::OpenGLES:
        break;
    case GraphicsDriverAPI::DX11:
        break;
    case GraphicsDriverAPI::DX12:
        break;
    case GraphicsDriverAPI::Vulkan:
        break;
    case GraphicsDriverAPI::Metal:
        break;
    }
    return nullptr;
}