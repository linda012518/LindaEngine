#include "TextureManager.h"
#include "YamlSerializer.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "TextureDriver.h"

#include <iostream>

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Texture>> TextureManager::_textureMap;
std::vector<Ref<RenderTexture>> RenderTextureManager::_renderTextures;

Ref<Texture> TextureManager::GetTexture(std::string& path)
{
    try
    {
        auto itr = _textureMap.find(path);
        if (itr == _textureMap.end())
        {
            if (path == "white" || path == "black" || path == "gray" || path == "bump")
            {
                _textureMap[path] = TextureLoader::Load(path);
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

void TextureManager::DeleteTexture(Ref<Texture> texture)
{
    for (auto itr = _textureMap.begin(); itr != _textureMap.end(); ) {
        if (itr->second != texture) {
            ++itr;
            continue;
        }
        _textureMap.erase(itr);
        TextureLoader::Delete(itr->second);
        break;
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

void TextureManager::Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex)
{
    TextureDriver::Bind(texture, channel, renderTextureColorIndex);
}

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, std::vector<FramebufferTextureSpecification>& fts, int msaa, int mipCount, bool isCube, bool isGammaCorrection, int anisotropy)
{
    for (auto& rt : _renderTextures)
    {
        if (rt->width == width && rt->height == height && rt->isCube == isCube && rt->isGammaCorrection == isGammaCorrection &&
            rt->msaa == msaa && rt->mipmapCount == mipCount && rt->anisotropy == anisotropy)
        {
            if (false == CompareAttachments(fts, rt->attachments))
                continue;

            auto itr = std::find(_renderTextures.begin(), _renderTextures.end(), rt);
            auto go = rt;
            _renderTextures.erase(itr);
            return go;
        }
    }

    Ref<RenderTexture> newRT = CreateRef<RenderTexture>();
    newRT->width = width;
    newRT->height = height;
    newRT->isCube = isCube;
    newRT->isGammaCorrection = isGammaCorrection;
    newRT->msaa = msaa;
    newRT->mipmapCount = mipCount;
    newRT->anisotropy = anisotropy;
    newRT->attachments = fts;

    for (auto& attachment : fts)
    {
        if (attachment.colorFormat == TextureFormat::Depth16 || attachment.colorFormat == TextureFormat::Depth24 || attachment.colorFormat == TextureFormat::Depth32 ||
            attachment.colorFormat == TextureFormat::Depth24Stencil8 || attachment.colorFormat == TextureFormat::Depth32Stencil8)
            newRT->depthAttachment = attachment;
        else
            newRT->colorAttachments.push_back(attachment);
    }

    TextureDriver::CreateRenderTexture(newRT);
    return newRT;
}

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, FramebufferTextureSpecification& fts, int msaa, int mipCount, bool isCube, bool isGammaCorrection, int anisotropy)
{
    std::vector<FramebufferTextureSpecification> array { fts };
    return Get(width, height, array, msaa, mipCount, isCube, isGammaCorrection, anisotropy);
}

Ref<RenderTexture> RenderTextureManager::GetBlitRenderTexture(Ref<RenderTexture> rt)
{
    if (rt->msaa == 1)
        return rt;
    else if (rt->msaa > 1)
    {
        if (nullptr == rt->internalRT)
        {
            std::vector<FramebufferTextureSpecification> fts;
            for (auto& go : rt->colorAttachments)
            {
                fts.push_back(go);
            }
            fts.push_back(rt->depthAttachment);

            rt->internalRT = Get(rt->width, rt->height, fts, 1, rt->mipmapCount, rt->isCube, rt->isGammaCorrection, rt->anisotropy);
        }
        TextureDriver::CopyRenderTexture(rt, rt->internalRT);
        return rt->internalRT;
    }
    else
    {
        std::cout << "RenderTextureManager::GetBlitRenderTexture Error this rt->mass < 0 !!!\n" << std::endl;
        return nullptr;
    }
}

void RenderTextureManager::Release(Ref<RenderTexture> rt)
{
    _renderTextures.push_back(rt);
    SetRenderTarget(nullptr);
}

void RenderTextureManager::Clear() 
{ 
    for (auto& rt : _renderTextures)
    {
        TextureDriver::DeleteRenderTexture(rt);
    }
    _renderTextures.clear(); 
}

void RenderTextureManager::SetRenderTarget(Ref<RenderTexture> texture)
{
    TextureDriver::BindRenderTarget(texture);
}

bool RenderTextureManager::CompareAttachments(std::vector<FramebufferTextureSpecification>& left, std::vector<FramebufferTextureSpecification>& right)
{
    int size = (int)left.size();
    if (size != right.size())
        return false;

    for (int i = 0; i < size; i++)
    {
        if (left[i].colorFormat != right[i].colorFormat || left[i].filter != right[i].filter ||
            left[i].warpU != right[i].warpU || left[i].warpV != right[i].warpV || left[i].warpW != right[i].warpW)
            return false;
    }
    return true;
}
