#include "RenderAPIContext.h"
#include "GraphicsContext.h"
#include "OpenglAPIContext.h"

using namespace LindaEngine;

//实现后期用宏定义割开
Scope<RenderAPIContext> RenderAPIContext::Create()
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateScope<OpenglAPIContext>();
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