#include "FBXManager.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<FBXResources>> FBXManager::_map;

Ref<FBXResources> FBXManager::GetFBX(std::string fbxPath)
{
    try
    {
        auto itr = _map.find(fbxPath);
        if (itr == _map.end())
        {
            _map[fbxPath] = FBXLoader::LoadFBX(fbxPath);
        }

        return _map[fbxPath];
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

Ref<Mesh> FBXManager::GetMesh(std::string fbxPath, std::string hashCode)
{
    try
    {
		return GetMeshFromFBX(GetFBX(fbxPath), hashCode);
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

void FBXManager::DestoryFBX(std::string fbxPath)
{
	if (_map.find(fbxPath) != _map.end())
	{
		ClearFBX(_map[fbxPath]);
        _map.erase(fbxPath);
	}
}

void FBXManager::Clear()
{
	_map.clear();
}

Ref<Mesh> FBXManager::GetMeshFromFBX(Ref<FBXResources> res, std::string hashCode)
{
    if (nullptr == res) return nullptr;

    if (res->hashCode == hashCode)
        return res->mesh;

    for (auto& go : res->children)
    {
        Ref<Mesh> temp = GetMeshFromFBX(go, hashCode);
        if (nullptr != temp) return temp;
    }

    return nullptr;
}

void FBXManager::ClearFBX(Ref<FBXResources> res)
{
	FBXResources* temp = res.get();
	for (auto& go : temp->children)
	{
		ClearFBX(go);
	}
	temp = nullptr;
	res = nullptr;
}

Ref<Mesh> FBXManager::GetSkybox()
{
	Ref<Mesh> mesh = CreateRef<Mesh>();

	Mesh::Data& data = mesh->AddMeshData(Mesh::Data());

	data.AddAttribute(VertexAttributeType::Position);

	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f, -1.0f });

	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f,  1.0f });

	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });

	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f, 1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f,  1.0f, 1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f, 1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f,  1.0f, 1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, 1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f, 1.0f });

	data.vertexData.insert(data.vertexData.end(), { -1.0f, 1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, 1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, 1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, 1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, 1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, 1.0f, -1.0f });

	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { -1.0f, -1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), {  1.0f, -1.0f,  1.0f });

	return mesh;
}

Ref<Mesh> FBXManager::GetEmpty()
{
	Ref<Mesh> mesh = CreateRef<Mesh>();
	Mesh::Data& data = mesh->AddMeshData(Mesh::Data());
	return mesh;
}

