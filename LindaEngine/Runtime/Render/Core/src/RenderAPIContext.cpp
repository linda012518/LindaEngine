#include "RenderAPIContext.h"

#include "OpenglAPIContext.h"

using namespace LindaEngine;

//实现后期用宏定义割开
Ref<RenderAPIContext> RenderAPIContext::Create(GraphicsDriverAPI api)
{
    switch (api)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenglAPIContext>();
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