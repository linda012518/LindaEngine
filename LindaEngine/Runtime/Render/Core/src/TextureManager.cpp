#include "TextureManager.h"
#include "TextureLoader.h"
#include "Texture.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Texture2D>> TextureManager::_textureMap;

Ref<Texture2D> TextureManager::GetTexture(const char* path)
{
    auto itr = _textureMap.find(path);
    if (itr == _textureMap.end())
    {
        _textureMap[path] = TextureLoader::Load(path);
    }

    return _textureMap[path];
}
