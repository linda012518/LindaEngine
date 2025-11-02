#include "MaterialManager.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "MaterialPass.h"
#include "ShaderManager.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Material>> MaterialManager::_materialMap;
std::unordered_map<std::string, Ref<Material>> MaterialManager::_defaultMaterial;
std::unordered_map<std::string, Ref<MaterialPass>> MaterialManager::_defaultPass;
bool MaterialManager::_isLoadDefault = false;

Ref<Material> MaterialManager::GetMaterial(const char* path)
{
    try
    {
        auto itr = _materialMap.find(path);
        if (itr == _materialMap.end())
        {
            _materialMap[path] = YamlSerializer::DeSerializeMaterial(path);
            _materialMap[path]->SetPath(path);
        }

        return _materialMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

Ref<Material> MaterialManager::GetMaterialByShader(const char* path)
{
    Ref<ShaderSource> ss = ShaderManager::GetShaderSource(path);
    Ref<Material> material = CreateRef<Material>();
    material->_state = ss->state;
    for (auto& pass : ss->shaderSrcCode)
    {
        Ref<MaterialPass> matPass = CreateRef<MaterialPass>();
        matPass->_state = pass->passState;
        material->_passes[matPass->_state.lightMode] = matPass;
    }
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

Ref<Material> MaterialManager::GetDefaultMaterial(const char* path)
{
    if (_defaultMaterial.find(path) == _defaultMaterial.end())
        return nullptr;
    return _defaultMaterial[path];
}

bool MaterialManager::LoadDefaultMaterial()
{
    //TODO 加载默认材质和默认Pass
    return true;
}
