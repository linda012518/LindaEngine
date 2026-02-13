#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "TextLoader.h"
#include "Shader.h"
#include "Mesh.h"
#include "Path.h"
#include "yaml-cpp/yaml.h"
#include "Application.h"
#include "Material.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<ShaderSource>> ShaderManager::_shaderSrcMap;
std::unordered_map<std::string, std::string> ShaderManager::_includes;
std::vector<std::string> ShaderManager::defaultAttributeNames;
std::string ShaderManager::defaultShaderUniformBlack;
std::string ShaderManager::defaultShaderUniform;
std::string ShaderManager::defaultShaderVersion;
std::string ShaderManager::FragmentOut;
std::string ShaderManager::PickVertexUniform;
std::string ShaderManager::PickVertexOut;
std::string ShaderManager::PickFragmentUniform;
std::string ShaderManager::PickFragmentOut;

bool _isLoadDefault = false;

Ref<ShaderSource> ShaderManager::GetShaderSource(const char* path)
{
    try
    {
        Initialize();

        auto itr = _shaderSrcMap.find(path);
        if (itr == _shaderSrcMap.end())
        {
            _shaderSrcMap[path] = ShaderLoader::Load(path);
        }

        return _shaderSrcMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
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

    std::string vertexShader = sss->vertex;
    std::string fragmentShader = sss->fragment;

    std::string fragmentOut = FragmentOut;

    //if (Application::module == AppModule::Editor)
    //{
    //    vertexShader = PickVertexUniform + vertexShader;
    //    ShaderLoader::AddPickOut(PickVertexOut, vertexShader);
    //    fragmentShader = "layout (location = " + std::to_string(sss->outColorCount) +") " + PickFragmentUniform + fragmentShader;
    //    ShaderLoader::AddPickOut(PickFragmentOut, fragmentShader);

    //    if (true == Material::isPickPass)
    //        fragmentOut = "";
    //}

    
    std::string tempVertex = defaultShaderVersion + defaultShaderUniformBlack + kw + layout + defaultShaderUniform + vertexShader;
    std::string tempFragment = defaultShaderVersion + defaultShaderUniformBlack + kw + defaultShaderUniform + fragmentOut + fragmentShader;
    return CreateRef<Shader>(tempVertex.c_str(), tempFragment.c_str());
}

bool ShaderManager::Initialize()
{
    if (true == _isLoadDefault)
        return true;
    _isLoadDefault = true;

    YAML::Node data;
    try
    {
        data = YAML::LoadFile(Path::shaderConfig);

        data = data["ShaderConfig"];
        if (!data)
            return false;

        PickVertexUniform = TextLoader::Load(data["PickVertexUniform"].as<std::string>().c_str());
        PickVertexOut = TextLoader::Load(data["PickVertexOut"].as<std::string>().c_str());
        PickFragmentUniform = TextLoader::Load(data["PickFragmentUniform"].as<std::string>().c_str());
        PickFragmentOut = TextLoader::Load(data["PickFragmentOut"].as<std::string>().c_str());
        FragmentOut = TextLoader::Load(data["FragmentOut"].as<std::string>().c_str());
        defaultShaderUniform = TextLoader::Load(data["BuiltInUniform"].as<std::string>().c_str());
        defaultShaderUniformBlack = TextLoader::Load(data["GlobalUniformBlock"].as<std::string>().c_str());
        defaultShaderVersion = TextLoader::Load(data["ShaderVersion"].as<std::string>().c_str());
        std::string names = TextLoader::Load(data["AttributeNames"].as<std::string>().c_str());

        names.erase(std::remove_if(names.begin(), names.end(), isspace), names.end());
        std::string token;
        std::istringstream tokenStream(names);
        while (std::getline(tokenStream, token, ',')) {
            defaultAttributeNames.push_back(token);
        }

        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
