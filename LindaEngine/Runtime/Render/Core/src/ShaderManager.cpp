#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "TextLoader.h"
#include "Shader.h"
#include "Mesh.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<ShaderSource>> ShaderManager::_shaderSrcMap;
std::unordered_map<std::string, std::string> ShaderManager::_includes;
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

void ShaderManager::Clear()
{
    _shaderSrcMap.clear();
    _includes.clear();
}

Ref<Shader> ShaderManager::CompileShader(Ref<ShaderSourceCode> sss, std::vector<std::string> &keywords, const std::vector<VertexAttribute>& attributes)
{
    std::string kw;
    for (auto& keyword : keywords)
    {
        kw += "#define " + keyword + " \n";
    }

    std::string layout;
    int index = 0;
    for (auto& attr : attributes)
    {
        std::string type = VertexAttribute::GetVertexDataTypeName(attr.dateType);
        layout += "layout (location = " + std::to_string(index) + ") in " + type + attr.attributeName + ";\n";
        index++;
    }

    std::string tempVertex = ShaderManager::defaultShaderVersion + ShaderManager::defaultShaderUniformBlack + kw + layout + sss->vertex;
    std::string tempFragment = ShaderManager::defaultShaderVersion + ShaderManager::defaultShaderUniformBlack + kw + sss->fragment;
    return CreateRef<Shader>(tempVertex.c_str(), tempFragment.c_str());
}
