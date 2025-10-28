#include "UniformBuffer.h"
#include "GraphicsContext.h"
#include "OpenGLUniformBuffer.h"

using namespace LindaEngine;

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenGLUniformBuffer>(size, binding);
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
