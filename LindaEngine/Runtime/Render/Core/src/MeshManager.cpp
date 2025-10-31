#include "MeshManager.h"
#include "MeshLoader.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<Mesh>> MeshManager::_meshMap;

Ref<Mesh> MeshManager::GetMesh(const char* path)
{
    try
    {
        auto itr = _meshMap.find(path);
        if (itr == _meshMap.end())
        {
            _meshMap[path] = MeshLoader::Load(path);
            _meshMap[path]->SetPath(path);
        }

        return _meshMap[path];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

Ref<Mesh> MeshManager::GetSphereMesh()
{
    return GetMesh("Sphere");
}

Ref<Mesh> MeshManager::GetCube()
{
    return GetMesh("Cube");
}

Ref<Mesh> MeshManager::GetCapsule()
{
    return GetMesh("Capsule");
}

Ref<Mesh> MeshManager::GetPlane()
{
    return GetMesh("Plane");
}

Ref<Mesh> MeshManager::GetSkybox()
{
    return GetMesh("Skybox");
}
