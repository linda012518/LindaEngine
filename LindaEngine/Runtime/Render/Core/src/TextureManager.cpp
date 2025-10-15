#include "TextureManager.h"
#include "TextureLoader.h"

using namespace LindaEngine;

std::unordered_map<std::string, unsigned int> TextureManager::_textureMap;

unsigned int TextureManager::GetTexture(const char* path)
{
    auto itr = _textureMap.find(path);
    if (itr == _textureMap.end())
    {
        _textureMap[path] = TextureLoader::Load(path);
    }

    return _textureMap[path];
}
