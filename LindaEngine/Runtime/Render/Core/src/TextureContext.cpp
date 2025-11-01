#include "TextureContext.h"
#include "GraphicsContext.h"
#include "OpenglTexture.h"

using namespace LindaEngine;

Ref<TextureContext> TextureContext::Create()
{
    switch (GraphicsContext::graphicsConfig.graphicsAPI)
    {
    case GraphicsDriverAPI::OpenGL:
        return CreateRef<OpenglTexture>();
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
