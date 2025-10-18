#include "TextureManager.h"
#include "TextureLoader.h"
#include "Texture.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Texture2D>> TextureManager::_textureMap;
std::vector<Ref<RenderTexture>> RenderTextureManager::_renderTextures;

Ref<Texture2D> TextureManager::GetTexture(const char* path)
{
    auto itr = _textureMap.find(path);
    if (itr == _textureMap.end())
    {
        _textureMap[path] = TextureLoader::Load(path);
    }

    return _textureMap[path];
}

void TextureManager::Clear() { _textureMap.clear(); }

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, TextureFormat colorFormat, bool sRGB, int msaa, int mipCount, TextureFormat depthFormat, TextureFormat stencilFormat)
{
    for (auto& rt : _renderTextures)
    {
        if (rt->width != width || rt->height != height || rt->colorFormat != colorFormat || rt->isDataSRGB != sRGB || 
            rt->msaa != msaa || rt->mipmapCount != mipCount || rt->depthFormat != depthFormat || rt->stencilFormat != stencilFormat)
        {
            Ref<RenderTexture> newRT = CreateRef<RenderTexture>();
            newRT->width = width;
            newRT->height = height;
            newRT->colorFormat = colorFormat;
            newRT->isDataSRGB = sRGB;
            newRT->msaa = msaa;
            newRT->mipmapCount = mipCount;
            newRT->depthFormat = depthFormat;
            newRT->stencilFormat = stencilFormat;
            return newRT;
        }

        auto itr = std::find(_renderTextures.begin(), _renderTextures.end(), rt);
        _renderTextures.erase(itr);
        return rt;
    }

    return nullptr;
}

void RenderTextureManager::Release(Ref<RenderTexture> rt)
{
    _renderTextures.push_back(rt);
}

void RenderTextureManager::Clear() { _renderTextures.clear(); }
