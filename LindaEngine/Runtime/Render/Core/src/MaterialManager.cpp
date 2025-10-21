#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Material>> MaterialManager::_materialMap;

Ref<Material> MaterialManager::GetMaterial(const char* path)
{
    auto itr = _materialMap.find(path);
    if (itr == _materialMap.end())
    {
        _materialMap[path] = YamlSerializer::DeSerializeMaterial(path);
    }

    return _materialMap[path];
}

void MaterialManager::Clear()
{
    _materialMap.clear();
}
