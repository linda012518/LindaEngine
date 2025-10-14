#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "TextLoader.h"

using namespace LindaEngine;

std::unordered_map<std::string, std::vector<ShaderSource>> ShaderManager::_shaderSrcMap;
std::unordered_map<std::string, std::string> ShaderManager::_includes;
std::vector<std::string> ShaderManager::defaultAttributeNames = { "aPosition", "aNormal", "aTangent", "aUV0", "aUV1", "aUV2", "aUV3", "aUV4", "aUV5", "aUV6", "aUV7", "aColor", "aBoneID1", "aBoneID2", "aBoneWeights1", "aBoneWeights2" };

std::vector<ShaderSource>& ShaderManager::GetShaderSource(const char* path)
{
    auto itr = _shaderSrcMap.find(path);
    if (itr == _shaderSrcMap.end())
    {
        _shaderSrcMap[path] = ShaderLoader::Load(path);
    }

    return _shaderSrcMap[path];
}

std::string& ShaderManager::GetInclude(const char* path)
{
    auto itr = _includes.find(path);
    if (itr == _includes.end())
    {
        _includes[path] = TextLoader::Load(path);
    }

    return _includes[path];
}
