#include "TextureManager.h"
#include "YamlSerializer.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "TextureDriver.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Texture>> TextureManager::_textureMap;
std::vector<Ref<RenderTexture>> RenderTextureManager::_renderTextures;

Ref<Texture> TextureManager::GetTexture(std::string path)
{
    try
    {
        auto itr = _textureMap.find(path);
        if (itr == _textureMap.end())
        {
            if (path == "white" || path == "black" || path == "gray" || path == "bump")
            {
                _textureMap[path] = TextureLoader::Load(path.c_str());
            }
            else
            {
                Ref<Texture> texture = YamlSerializer::DeSerializeTexture(path.c_str());
                if (nullptr == texture)
                    return nullptr;
                TextureLoader::Load(texture);
                _textureMap[path] = texture;
            }
        }

        return _textureMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

void TextureManager::Clear() 
{ 
    for (auto& tex : _textureMap)
    {
        TextureLoader::Delete(tex.second);
    }
    _textureMap.clear(); 
}

void TextureManager::Bind(Ref<Texture> texture, int channel)
{
    TextureDriver::Bind(texture, channel);
}

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, TextureFormat colorFormat, bool sRGB, int msaa, int mipCount, TextureFormat depthFormat, TextureFormat stencilFormat)
{
    for (auto& rt : _renderTextures)
    {
        if (rt->width != width || rt->height != height || rt->colorFormat != colorFormat || rt->isGammaCorrection != sRGB ||
            rt->msaa != msaa || rt->mipmapCount != mipCount || rt->depthFormat != depthFormat || rt->stencilFormat != stencilFormat)
        {
            Ref<RenderTexture> newRT = CreateRef<RenderTexture>();
            newRT->width = width;
            newRT->height = height;
            newRT->colorFormat = colorFormat;
            newRT->isGammaCorrection = sRGB;
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
