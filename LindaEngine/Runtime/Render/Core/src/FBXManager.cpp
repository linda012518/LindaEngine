#include "FBXManager.h"
#include "Light.h"
#include "glm/glm.hpp"

using namespace LindaEngine;

std::unordered_map<std::string, Ref<FBXResources>> FBXManager::_map;
Ref<Mesh> FBXManager::_skybox = nullptr;
Ref<Mesh> FBXManager::_sphere = nullptr;
Ref<Mesh> FBXManager::_empty = nullptr;
Ref<Mesh> FBXManager::_boundingBox = nullptr;
Ref<Mesh> FBXManager::_frustumMesh = nullptr;
Ref<Mesh> FBXManager::_directionLightMesh = nullptr;
Ref<Mesh> FBXManager::_pointLightMesh = nullptr;
Ref<Mesh> FBXManager::_spotLightMesh = nullptr;

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

std::vector<BoneData> FBXManager::GetMeshBoneData(std::string fbxPath, std::string hashCode)
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

Ref<Mesh> FBXManager::CreateDirectionLightMesh()
{
	if (nullptr != _directionLightMesh)
		return _directionLightMesh;

	_directionLightMesh = CreateRef<Mesh>();
	Mesh::Data& data = _directionLightMesh->AddMeshData(Mesh::Data());
	data.AddAttribute(VertexAttributeType::Position);
	data.drawType = DrawType::LINES;

	// 默认光方向为 Z 轴正向（使用时可整体旋转对象来匹配实际方向）
	const glm::vec3 dir(0.0f, 0.0f, 1.0f);
	const glm::vec3 right(1.0f, 0.0f, 0.0f);
	const glm::vec3 up(0.0f, 1.0f, 0.0f);

	float scale = 0.2f;
	float radius = 1.0f * scale; // 半径
	float lineLength = 5.0f * scale; // 轴线长度（从圆心到箭头尖端）
	float arrowHeight = 2.0f * scale; // 箭头高度（顶点到底面平面的距离）
	float arrowWidth = 1.0f * scale; // 箭头底面正方形的宽度（控制射线端点距离轴线的程度）
	int segments = 12;

	// --- 顶点定义 -------------------------------------------------
	// 0: 圆心
	data.AddFloat3(glm::vec3(0.0f));

	// 1: 箭头顶点（轴线末端）
	glm::vec3 tip = dir * lineLength;
	data.AddFloat3(tip);

	// 箭头底面中心（用于计算四个射线端点）
	glm::vec3 baseCenter = tip - dir * arrowHeight;

	// 2~5: 底面四个顶点（射线端点）
	data.AddFloat3(baseCenter + right * arrowWidth * 0.5f); // 2: 右
	data.AddFloat3(baseCenter + up * arrowWidth * 0.5f); // 3: 上
	data.AddFloat3(baseCenter - right * arrowWidth * 0.5f); // 4: 左
	data.AddFloat3(baseCenter - up * arrowWidth * 0.5f); // 5: 下

	// 圆周点（索引从 6 开始）
	int circleStartIdx = 6;
	for (int i = 0; i < segments; ++i) {
		float angle = static_cast<float>(i) / segments * glm::two_pi<float>();
		glm::vec3 offset = right * cos(angle) * radius + up * sin(angle) * radius;
		data.AddFloat3(offset);
	}

	// --- 线段索引构建 ---------------------------------------------
	// 1. 轴线（圆心到顶点）
	data.indexData.push_back(0);
	data.indexData.push_back(1);

	// 2. 四射线箭头：顶点到四个端点（四条侧棱）
	data.indexData.push_back(1); data.indexData.push_back(2);
	data.indexData.push_back(1); data.indexData.push_back(3);
	data.indexData.push_back(1); data.indexData.push_back(4);
	data.indexData.push_back(1); data.indexData.push_back(5);

	// 3. 圆周环线
	for (int i = 0; i < segments; ++i) {
		unsigned int curr = circleStartIdx + i;
		unsigned int next = circleStartIdx + (i + 1) % segments;
		data.indexData.push_back(curr);
		data.indexData.push_back(next);
	}

	// 4. 辐射线（圆心到每个圆周点）
	for (int i = 0; i < segments; ++i) {
		data.indexData.push_back(0);
		data.indexData.push_back(circleStartIdx + i);
	}

	return _directionLightMesh;
}

Ref<Mesh> FBXManager::CreatePointLightMesh()
{
	if (nullptr != _pointLightMesh)
		return _pointLightMesh;

	_pointLightMesh = CreateRef<Mesh>();
	Mesh::Data& data = _pointLightMesh->AddMeshData(Mesh::Data());
	data.AddAttribute(VertexAttributeType::Position);
	data.drawType = DrawType::LINES;

	float scale = 1.0f;
	float radius = 1.0f * scale; // 半径
	int segments = 36;
	float angleStep = glm::two_pi<float>() / segments;

	// --- 顶点定义 -------------------------------------------------
	// 圆1：XY 平面（绕 Z 轴），顶点坐标 (r*cos, r*sin, 0)
	for (int i = 0; i < segments; ++i) {
		float angle = i * angleStep;
		data.AddFloat3(glm::vec3(radius * cos(angle), radius * sin(angle), 0.0f));
	}
	// 圆2：XZ 平面（绕 Y 轴），顶点坐标 (r*cos, 0, r*sin)
	for (int i = 0; i < segments; ++i) {
		float angle = i * angleStep;
		data.AddFloat3(glm::vec3(radius * cos(angle), 0.0f, radius * sin(angle)));
	}
	// 圆3：YZ 平面（绕 X 轴），顶点坐标 (0, r*cos, r*sin)
	for (int i = 0; i < segments; ++i) {
		float angle = i * angleStep;
		data.AddFloat3(glm::vec3(0.0f, radius * cos(angle), radius * sin(angle)));
	}

	// --- 线段索引构建 ---------------------------------------------
	// 每个圆独立构成闭合环
	for (int ring = 0; ring < 3; ++ring) {
		int startIdx = ring * segments;          // 当前圆在顶点数组中的起始索引
		for (int i = 0; i < segments; ++i) {
			unsigned int curr = startIdx + i;
			unsigned int next = startIdx + (i + 1) % segments;
			data.indexData.push_back(curr);
			data.indexData.push_back(next);
		}
	}

	return _pointLightMesh;
}

Ref<Mesh> FBXManager::CreateSpotLightMesh()
{
	if (nullptr != _spotLightMesh)
		return _spotLightMesh;

	_spotLightMesh = CreateRef<Mesh>();
	Mesh::Data& data = _spotLightMesh->AddMeshData(Mesh::Data());
	data.AddAttribute(VertexAttributeType::Position);
	data.drawType = DrawType::LINES;

	int segments = 36;
	const float angleStep = glm::two_pi<float>() / segments;

	// --- 顶点定义 -------------------------------------------------
	// 0: 顶点（光源位置）
	data.AddFloat3(glm::vec3(0.0f, 0.0f, 0.0f));
	// 1: 底面中心
	data.AddFloat3(glm::vec3(0.0f, 0.0f, 1.0f));

	// 2 ~ 2+segments-1: 底面圆周点 (z = 1)
	for (int i = 0; i < segments; ++i) {
		float angle = i * angleStep;
		float x = cos(angle);
		float y = sin(angle);
		data.AddFloat3(glm::vec3(x, y, 1.0f));
	}

	// --- 线段索引构建 ---------------------------------------------
	const int baseIdx = 2;  // 第一个圆周点的索引

	// 轴线 (0 → 1)
	data.indexData.push_back(0);
	data.indexData.push_back(1);

	// 母线 (0 → 每个圆周点)
	for (int i = 0; i < segments; ++i) {
		data.indexData.push_back(0);
		data.indexData.push_back(baseIdx + i);
	}

	// 底面圆环 (相邻圆周点相连)
	for (int i = 0; i < segments; ++i) {
		int curr = baseIdx + i;
		int next = baseIdx + (i + 1) % segments;
		data.indexData.push_back(curr);
		data.indexData.push_back(next);
	}

	//// 辐条 (底面中心 → 每个圆周点)
	//for (int i = 0; i < segments; ++i) {
	//	data.indexData.push_back(1);
	//	data.indexData.push_back(baseIdx + i);
	//}


	return _spotLightMesh;
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

Ref<Mesh> FBXManager::GetShpere()
{
	if (nullptr != _sphere)
		return _sphere;

	_sphere = CreateRef<Mesh>();
	Mesh::Data& data = _sphere->AddMeshData(Mesh::Data());
	data.AddAttribute(VertexAttributeType::Position);
	data.AddAttribute(VertexAttributeType::Normal);
	data.AddAttribute(VertexAttributeType::Tangent);
	data.AddAttribute(VertexAttributeType::UV0);
	data.drawType = DrawType::TRIANGLE_STRIP;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			const float theta = xSegment * 2.0f * PI;
			const float phi = ySegment * PI;
			const float sinPhi = std::sin(phi);
			float xPos = std::cos(theta) * sinPhi;
			float yPos = std::cos(phi);
			float zPos = std::sin(theta) * sinPhi;

			// Tangent along increasing u (theta); degenerates at poles where sin(phi)==0.
			glm::vec3 tanU(-std::sin(theta) * sinPhi, 0.0f, std::cos(theta) * sinPhi);
			const float tanLenSq = tanU.x * tanU.x + tanU.z * tanU.z;
			if (tanLenSq > 1e-12f)
				tanU = tanU / std::sqrt(tanLenSq);
			else
				tanU = glm::vec3(1.0f, 0.0f, 0.0f);

			data.AddFloat3(glm::vec3(xPos, yPos, zPos));
			data.AddFloat3(glm::vec3(xPos, yPos, zPos));
			data.AddFloat3(tanU);
			data.AddFloat2(glm::vec2(xSegment, ySegment));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				data.indexData.push_back(y * (X_SEGMENTS + 1) + x);
				data.indexData.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				data.indexData.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				data.indexData.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	return _sphere;
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

Ref<Mesh> FBXManager::GetLightMesh(LightType type)
{
	switch (type)
	{
	case LightType::DirectionLight:
		return CreateDirectionLightMesh();
	case LightType::SpotLight:
		return CreateSpotLightMesh();
	case LightType::PointLight:
		return CreatePointLightMesh();
	}

	return nullptr;
}

