#include "FBXManager.h"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<FBXResources>> FBXManager::_map;
Ref<Mesh> FBXManager::_skybox = nullptr;
Ref<Mesh> FBXManager::_empty = nullptr;
Ref<Mesh> FBXManager::_boundingBox = nullptr;
Ref<Mesh> FBXManager::_frustumMesh = nullptr;

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
		Ref<FBXResources> resNode = GetFBXResources(GetFBX(fbxPath), hashCode);
		if (nullptr == resNode)
			return nullptr;
		return resNode->mesh;
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

std::vector<BoneData>& FBXManager::GetMeshBoneData(std::string fbxPath, std::string hashCode)
{
	std::vector<BoneData> data;
	try
	{
		Ref<FBXResources> resNode = GetFBXResources(GetFBX(fbxPath), hashCode);
		if (nullptr == resNode)
			return data;
		return resNode->bones;
	}
	catch (const std::exception&)
	{
		return data;
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

Ref<FBXResources> FBXManager::GetFBXResources(Ref<FBXResources> res, std::string hashCode)
{
	if (nullptr == res) return nullptr;

	if (res->hashCode == hashCode)
		return res;

	for (auto& go : res->children)
	{
		Ref<FBXResources> temp = GetFBXResources(go, hashCode);
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

Ref<Mesh> FBXManager::CreateCubeMesh(DrawType type, float scale)
{
	Ref<Mesh> mesh = CreateRef<Mesh>();
	Mesh::Data& data = mesh->AddMeshData(Mesh::Data());
	data.AddAttribute(VertexAttributeType::Position);
	data.drawType = type;

	float value = 1.0f * scale;

	data.vertexData.insert(data.vertexData.end(), {  value,  value,  value });
	data.vertexData.insert(data.vertexData.end(), { -value,  value,  value });

	data.vertexData.insert(data.vertexData.end(), { -value,  value,  value });
	data.vertexData.insert(data.vertexData.end(), { -value, -value,  value });

	data.vertexData.insert(data.vertexData.end(), { -value, -value,  value });
	data.vertexData.insert(data.vertexData.end(), {  value, -value,  value });

	data.vertexData.insert(data.vertexData.end(), {  value, -value,  value });
	data.vertexData.insert(data.vertexData.end(), {  value,  value,  value });
	//////////////////////////////////////////////////////////////////////////////
	data.vertexData.insert(data.vertexData.end(), { value,  value, -value });
	data.vertexData.insert(data.vertexData.end(), { -value,  value, -value });

	data.vertexData.insert(data.vertexData.end(), { -value,  value, -value });
	data.vertexData.insert(data.vertexData.end(), { -value, -value, -value });

	data.vertexData.insert(data.vertexData.end(), { -value, -value, -value });
	data.vertexData.insert(data.vertexData.end(), {  value, -value, -value });

	data.vertexData.insert(data.vertexData.end(), {  value, -value, -value });
	data.vertexData.insert(data.vertexData.end(), {  value,  value, -value });
	//////////////////////////////////////////////////////////////////////////////
	data.vertexData.insert(data.vertexData.end(), {  value,  value,   value });
	data.vertexData.insert(data.vertexData.end(), {  value,  value,  -value });

	data.vertexData.insert(data.vertexData.end(), { -value,  value,   value });
	data.vertexData.insert(data.vertexData.end(), { -value,  value,  -value });

	data.vertexData.insert(data.vertexData.end(), { -value, -value,   value });
	data.vertexData.insert(data.vertexData.end(), { -value, -value,  -value });

	data.vertexData.insert(data.vertexData.end(), {  value, -value,   value });
	data.vertexData.insert(data.vertexData.end(), {  value, -value,  -value });

	return mesh;

}

Ref<Mesh> FBXManager::GetSkybox()
{
	if (nullptr != _skybox)
		return _skybox;

	_skybox = CreateRef<Mesh>();

	Mesh::Data& data = _skybox->AddMeshData(Mesh::Data());

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

	return _skybox;
}

Ref<Mesh> FBXManager::GetEmpty()
{
	if (nullptr != _empty)
		return _empty;

	_empty = CreateRef<Mesh>();
	Mesh::Data& data = _empty->AddMeshData(Mesh::Data());
	return _empty;
}

Ref<Mesh> FBXManager::GetBoundingBox()
{
	if (nullptr != _boundingBox)
		return _boundingBox;

	_boundingBox = CreateCubeMesh(DrawType::LINES, 0.5f);
	return _boundingBox;
}

Ref<Mesh> FBXManager::GetFrustumMesh()
{
	if (nullptr != _frustumMesh)
		return _frustumMesh;

	_frustumMesh = CreateCubeMesh(DrawType::LINES, 1.0f);
	return _frustumMesh;
}

Ref<Mesh> FBXManager::GetLightMesh()
{
	return Ref<Mesh>();
}

