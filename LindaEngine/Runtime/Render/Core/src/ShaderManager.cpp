#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "TextLoader.h"
#include "Shader.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<ShaderSource>> ShaderManager::_shaderSrcMap;
std::unordered_map<std::string, std::string> ShaderManager::_includes;
std::unordered_map<Ref<ShaderSourceCode>, Ref<Shader>> ShaderManager::_shaderProgramMap;
std::vector<std::string> ShaderManager::defaultAttributeNames = { "aPosition", "aNormal", "aTangent", "aUV0", "aUV1", "aUV2", "aUV3", "aUV4", "aUV5", "aUV6", "aUV7", "aColor", "aBoneID1", "aBoneID2", "aBoneWeights1", "aBoneWeights2" };
std::string ShaderManager::defaultShaderUniformBlack;
std::string ShaderManager::defaultShaderVersion = "#version 330 core\n";

bool _isLoadDefault = false;

Ref<ShaderSource>& ShaderManager::GetShaderSource(const char* path)
{
    if (false == _isLoadDefault)
    {
        //TODO º”‘ÿƒ¨»œshader
        _isLoadDefault = true;
    }

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
