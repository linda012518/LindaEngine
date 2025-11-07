#include "TextureManager.h"
#include "YamlSerializer.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "TextureDriver.h"

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
            _renderTextures.erase(itr);
            return rt;
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

void RenderTextureManager::Release(Ref<RenderTexture> rt)
{
    _renderTextures.push_back(rt);
}

void RenderTextureManager::Clear() 
{ 
    for (auto& rt : _renderTextures)
    {
        TextureDriver::DeleteRenderTexture(rt);
    }
    _renderTextures.clear(); 
}

bool RenderTextureManager::CompareAttachments(std::vector<FramebufferTextureSpecification>& left, std::vector<FramebufferTextureSpecification>& right)
{
    int size = left.size();
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
