#include "MaterialManager.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "MaterialPass.h"
#include "ShaderManager.h"
#include "Path.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Material>> MaterialManager::_materialMap;
std::unordered_map<std::string, Ref<Material>> MaterialManager::_defaultMaterial;
std::unordered_map<std::string, Ref<MaterialPass>> MaterialManager::_defaultPass;
bool MaterialManager::_isLoadDefault = false;

Ref<Material> MaterialManager::GetMaterial(std::string path, bool isSkin)
{
    try
    {
        std::string key = isSkin ? path + "isSkin" : path;
        auto itr = _materialMap.find(key);
        if (itr == _materialMap.end())
        {
            _materialMap[key] = YamlSerializer::DeSerializeMaterial(path.c_str());
            _materialMap[key]->SetPath(path.c_str());
        }
        return _materialMap[key];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

Ref<Material> MaterialManager::GetMaterialByShader(std::string path, bool isSkin)
{
    Ref<ShaderSource> ss = ShaderManager::GetShaderSource(path.c_str());
    Ref<Material> material = CreateRef<Material>();
    material->_state = ss->state;
    for (auto& pass : ss->shaderSrcCode)
    {
        Ref<MaterialPass> matPass = CreateRef<MaterialPass>();
        matPass->_state = pass->passState;
        material->_passes.push_back(matPass);
    }
    if (isSkin)
        material->AddKeyword("_Skin_Vertex_");
    return material;
}

void MaterialManager::Clear()
{
    _materialMap.clear();
}

Ref<MaterialPass> MaterialManager::GetDefaultMaterialPass(const char* lightMode)
{
    if (false == _isLoadDefault)
    {
        MaterialManager::LoadDefaultMaterial();
        _isLoadDefault = true;
    }

    if (_defaultPass.find(lightMode) == _defaultPass.end())
        return nullptr;

    return _defaultPass[lightMode];
}

Ref<Material> MaterialManager::GetDefaultMaterial(std::string path, bool isSkin)
{
    if (false == _isLoadDefault)
    {
        MaterialManager::LoadDefaultMaterial();
        _isLoadDefault = true;
    }

    if (isSkin)
        path += "isSkin";

    if (_defaultMaterial.find(path) == _defaultMaterial.end())
        return nullptr;

    return _defaultMaterial[path];
}

bool MaterialManager::LoadDefaultMaterial()
{
    //TODO 加载默认材质和默认Pass
    if (true == _isLoadDefault)
        return true;
    _isLoadDefault = true;

    YAML::Node data;
    try
    {
        data = YAML::LoadFile(Path::materialConfig);

        data = data["MaterialConfig"];
        if (!data)
            return false;

        for (std::size_t i = 0; i < data.size(); i++) 
        {
            std::string shaderPath = data[i].as<std::string>();
            auto pass1 = GetMaterialByShader(shaderPath);
            if (nullptr != pass1)
            {
                _defaultMaterial[shaderPath] = pass1;
            }

            auto pass2 = GetMaterialByShader(shaderPath, true);
            if (nullptr != pass2)
            {
                _defaultMaterial[shaderPath + "isSkin"] = pass2;
            }
        }

        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }

}
