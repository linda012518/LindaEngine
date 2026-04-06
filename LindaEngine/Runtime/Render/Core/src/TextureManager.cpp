#include "TextureManager.h"
#include "YamlSerializer.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "TextureDriver.h"

#include <iostream>

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Texture>> TextureManager::_textureMap;
std::vector<Ref<RenderTexture>> RenderTextureManager::_renderTextures;
std::vector<Ref<RenderTexture>> RenderTextureManager::_dirtyTextures;

Ref<Texture> TextureManager::GetTexture(std::string path, bool reload)
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
                std::string metaPath = path + ".meta";
                Ref<Texture> texture = YamlSerializer::DeSerializeTexture(metaPath.c_str());
                if (nullptr == texture)
                    return nullptr;
                TextureLoader::Load(texture);
                _textureMap[path] = texture;
            }
        }
        else if (true == reload)
        {
            std::string metaPath = path + ".meta";
            Ref<Texture> temp = Texture::overrideTexture;
            Texture::overrideTexture = _textureMap[path];
            YamlSerializer::SerializeTexture(metaPath.c_str());
            Texture::overrideTexture = temp;

            DeleteTexture(_textureMap[path]);

            Ref<Texture> texture = YamlSerializer::DeSerializeTexture(metaPath.c_str());
            if (nullptr == texture)
                return nullptr;
            TextureLoader::Load(texture);
            _textureMap[path] = texture;
        }

        return _textureMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

Ref<Texture> TextureManager::GetTextureDirect(std::string path)
{
    try
    {
        if (_textureMap.find(path) == _textureMap.end())
        {
            Ref<Texture2D> texture = CreateRef<Texture2D>();
            if (nullptr == texture)
                return nullptr;
            texture->path = path;
            TextureLoader::Load(texture);
            _textureMap[path] = texture;
        }

        return _textureMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

bool TextureManager::IsLoad(std::string path)
{
    return _textureMap.find(path) != _textureMap.end();
}

void TextureManager::DeleteTexture(Ref<Texture> texture)
{
    for (auto itr = _textureMap.begin(); itr != _textureMap.end(); ) {
        if (itr->second != texture) {
            ++itr;
            continue;
        }
        TextureLoader::Delete(itr->second);
        _textureMap.erase(itr);
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

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, std::vector<FramebufferTextureSpecification>& fts, int msaa, int mipCount, bool isCube, bool isGammaCorrection, int anisotropy, bool linkScreen)
{
    for (auto& rt : _renderTextures)
    {
        if (rt->width == width && rt->height == height && rt->isCube == isCube && rt->isGammaCorrection == isGammaCorrection &&
            rt->msaa == msaa && rt->mipmapCount == mipCount && rt->anisotropy == anisotropy && rt->isLinkScreen == linkScreen)
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
    newRT->isLinkScreen = linkScreen;
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

Ref<RenderTexture> RenderTextureManager::Get(int width, int height, FramebufferTextureSpecification& fts, int msaa, int mipCount, bool isCube, bool isGammaCorrection, int anisotropy, bool linkScreen)
{
    std::vector<FramebufferTextureSpecification> array { fts };
    return Get(width, height, array, msaa, mipCount, isCube, isGammaCorrection, anisotropy, linkScreen);
}

Ref<RenderTexture> RenderTextureManager::Get(Ref<RenderTexture> src)
{
    return Get(src->width, src->height, src->attachments, src->msaa, src->mipmapCount, src->isCube, src->isGammaCorrection, src->anisotropy);
}

Ref<RenderTexture> RenderTextureManager::GetBlitRenderTexture(Ref<RenderTexture> rt, ColorType type)
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
        TextureDriver::CopyRenderTexture(rt, rt->internalRT, type);
        return rt->internalRT;
    }
    else
    {
        std::cout << "RenderTextureManager::GetBlitRenderTexture Error this rt->mass < 0 !!!\n" << std::endl;
        return nullptr;
    }
}

void RenderTextureManager::BlitRenderTexture(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type)
{
    TextureDriver::CopyRenderTexture(src, dest, type);
}

void RenderTextureManager::Release(Ref<RenderTexture> rt)
{
    if (nullptr == rt)
        return;
    _dirtyTextures.push_back(rt);
}

void RenderTextureManager::DeleteImmediately(Ref<RenderTexture> rt)
{
    TextureDriver::DeleteRenderTexture(rt);
    if (rt->internalRT != nullptr)
    {
        TextureDriver::DeleteRenderTexture(rt->internalRT);
        rt->internalRT = nullptr;
    }
    rt = nullptr;
}

void RenderTextureManager::Clear() 
{ 
    for (auto& rt : _renderTextures)
    {
        DeleteImmediately(rt);
    }
    _renderTextures.clear(); 

    for (auto& rt : _dirtyTextures)
    {
        DeleteImmediately(rt);
    }
    _dirtyTextures.clear();
}

void RenderTextureManager::ClearLinkScreen()
{
    for (auto it = _renderTextures.begin(); it != _renderTextures.end();) 
    {
        Ref<RenderTexture> rt = *it;
        if (rt->isLinkScreen == true) {
            DeleteImmediately(rt);
            it = _renderTextures.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = _dirtyTextures.begin(); it != _dirtyTextures.end();)
    {
        Ref<RenderTexture> rt = *it;
        if (rt->isLinkScreen == true) {
            DeleteImmediately(rt);
            it = _dirtyTextures.erase(it);
        }
        else {
            ++it;
        }
    }
}

void RenderTextureManager::SetRenderTarget(Ref<RenderTexture> texture)
{
    RenderTexture::active = texture;
    TextureDriver::BindRenderTarget(texture);
}

void RenderTextureManager::Tick()
{
    for (auto& rt : _dirtyTextures)
    {
        _renderTextures.push_back(rt);
    }
    _dirtyTextures.clear();
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
