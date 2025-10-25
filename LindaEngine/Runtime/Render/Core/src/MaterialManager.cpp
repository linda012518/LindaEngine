#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "YamlSerializer.h"
#include "Material.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Material>> MaterialManager::_materialMap;
std::unordered_map<std::string, Ref<Material>> MaterialManager::_defaultMaterial;
std::unordered_map<std::string, Ref<MaterialPass>> MaterialManager::_defaultPass;
bool MaterialManager::_isLoadDefault = false;

Ref<Material> MaterialManager::GetMaterial(const char* path)
{
    auto itr = _materialMap.find(path);
    if (itr == _materialMap.end())
    {
        _materialMap[path] = YamlSerializer::DeSerializeMaterial(path);
        _materialMap[path]->SetPath(path);
    }

    return _materialMap[path];
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
