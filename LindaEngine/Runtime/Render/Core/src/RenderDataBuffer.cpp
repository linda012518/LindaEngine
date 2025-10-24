#include "RenderDataBuffer.h"
#include "GraphicsContext.h"
#include "OpenglRenderDataBuffer.h"

using namespace LindaEngine;

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size);
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

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
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

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(indices, count);
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

Ref<VertexArray> VertexArray::Create()
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenGLVertexArray>();
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
