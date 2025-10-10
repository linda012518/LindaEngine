#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "TextLoader.h"

using namespace LindaEngine;

std::unordered_map<std::string, std::vector<ShaderSource>> ShaderManager::_shaderSrcMap;
std::unordered_map<std::string, std::string> ShaderManager::_includes;

std::vector<ShaderSource>& ShaderManager::GetShaderSource(std::string& path)
{
    auto itr = _shaderSrcMap.find(path);
    if (itr == _shaderSrcMap.end())
    {
        _shaderSrcMap[path] = ShaderLoader::Load(path.c_str());
    }

    return _shaderSrcMap[path];
}

std::string& ShaderManager::GetInclude(std::string& path)
{
    auto itr = _includes.find(path);
    if (itr == _includes.end())
    {
        _includes[path] = TextLoader::Load(path.c_str());
    }

    return _includes[path];
}
