#include "MeshLoader.h"
#include "Mesh.h"
#include "Mathf.h"
#include "glm/glm.hpp"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <iostream>

using namespace LindaEngine;

#define MAX_BONE_INFLUENCE 4

std::unordered_map<std::string, BoneInfo> MeshLoader::_boneInfoMap;
int MeshLoader::_boneCounter = 0;

glm::vec3 CalculateTangent(glm::vec3& pos0, glm::vec3& pos1, glm::vec3& pos2, glm::vec2& uv0, glm::vec2& uv1, glm::vec2& uv2)
{
	glm::vec3 deltaPos1 = pos1 - pos0;
	glm::vec3 deltaPos2 = pos2 - pos0;

	glm::vec2 deltaUV1 = uv1 - uv0;
	glm::vec2 deltaUV2 = uv2 - uv0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	return (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
}

glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

Ref<Mesh> MeshLoader::Load(std::string path)
{
	if (path == "Skybox") return LoadSkybox();
	else if (path == "Empty") return LoadEmpty();

	Assimp::Importer importer;
	//aiProcess_Triangulate			如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
	//aiProcess_FlipUVs				在处理的时候翻转y轴的纹理坐标
	//aiProcess_GenNormals			如果模型不包含法向量的话，就为每个顶点创建法线
	//aiProcess_SplitLargeMeshes	将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用
	//aiProcess_OptimizeMeshes		和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	//mFlags	返回的数据是不是不完整的
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	Ref<Mesh> mesh = CreateRef<Mesh>();
	_boneInfoMap.clear();
	_boneCounter = 0;

	aiMatrix4x4 identity;
	ParseAssimpNode(&identity, scene->mRootNode, scene, mesh);

	//if ("BuiltInAssets/Meshs/DirectionLight.FBX" == path)
	//{
	//	mesh->GetMeshData()->drawType = DrawType::LINES;
	//}

	return mesh;
}

Ref<Mesh> MeshLoader::LoadSkybox()
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

	data.vertexData.insert(data.vertexData.end(), { 1.0f, -1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { 1.0f, -1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { 1.0f,  1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { 1.0f,  1.0f,  1.0f });
	data.vertexData.insert(data.vertexData.end(), { 1.0f,  1.0f, -1.0f });
	data.vertexData.insert(data.vertexData.end(), { 1.0f, -1.0f, -1.0f });

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

Ref<Mesh> MeshLoader::LoadEmpty()
{
	Ref<Mesh> mesh = CreateRef<Mesh>();
	Mesh::Data& data = mesh->AddMeshData(Mesh::Data());
	return mesh;
}

Ref<Mesh> MeshLoader::LoadCameraFrustum()
{
	return nullptr;
}

Ref<Mesh> MeshLoader::LoadSpotLightFrustum()
{
	return nullptr;
}

void MeshLoader::ParseAssimpNode(void* mat, aiNode* node, const aiScene* scene, Ref<Mesh> mesh)
{
	aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(mat) * node->mTransformation;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		//每个节点的网格索引，对应 场景的mMeshes数组的网格
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		ParseAssimpMesh(&nodeTransformation, aiMesh, scene, mesh->AddMeshData(Mesh::Data()), mesh);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ParseAssimpNode(&nodeTransformation, node->mChildren[i], scene, mesh);
	}
}

void MeshLoader::ParseAssimpMesh(void* mat, aiMesh* aiMesh, const aiScene* scene, Mesh::Data& meshData, Ref<Mesh> meshPtr)
{
	if (aiMesh->mNumVertices <= 3)
		return;

	meshData.AddAttribute(VertexAttributeType::Position);

	if (NULL != aiMesh->mNormals)
	{
		meshData.AddAttribute(VertexAttributeType::Normal);
		meshData.AddAttribute(VertexAttributeType::Tangent);
	}
	for (int i = 0; i < 8; i++)
	{
		if (NULL == aiMesh->mTextureCoords[i])
			break;
		meshData.AddAttribute("aUV" + std::to_string(i));
	}
	if (NULL != aiMesh->mColors[0])
		meshData.AddAttribute(VertexAttributeType::Color);

	if (aiMesh->mNumBones >= 1)
	{
		meshData.AddAttribute(VertexAttributeType::BoneWeights1);
		meshData.AddAttribute(VertexAttributeType::BoneID1);
	}

	int index = 0;
	meshData.vertexData.resize(((size_t)meshData.vertexStride / 4) * ((size_t)aiMesh->mNumVertices));

	aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(mat);

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		const aiVector3D position = meshTransformation * aiMesh->mVertices[i];
		meshData.vertexData[index++] = position.x;
		meshData.vertexData[index++] = position.y;
		meshData.vertexData[index++] = position.z;

		meshPtr->UpdateBoundingBox(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);

		if (NULL != aiMesh->mNormals)
		{
			const aiVector3D normal = meshTransformation * aiMesh->mNormals[i];
			const aiVector3D tangent = meshTransformation * aiMesh->mTangents[i];
			meshData.vertexData[index++] = normal.x;
			meshData.vertexData[index++] = normal.y;
			meshData.vertexData[index++] = normal.z;

			meshData.vertexData[index++] = tangent.x;
			meshData.vertexData[index++] = tangent.y;
			meshData.vertexData[index++] = tangent.z;
		}

		for (int n = 0; n < 8; n++)
		{
			if (NULL == aiMesh->mTextureCoords[n])
				break;

			meshData.vertexData[index++] = (aiMesh->mTextureCoords[n][i].x);
			meshData.vertexData[index++] = (aiMesh->mTextureCoords[n][i].y);
		}

		if (NULL != aiMesh->mColors[0])
		{
			meshData.vertexData[index++] = (aiMesh->mColors[0][i].r);
			meshData.vertexData[index++] = (aiMesh->mColors[0][i].g);
			meshData.vertexData[index++] = (aiMesh->mColors[0][i].b);
			meshData.vertexData[index++] = (aiMesh->mColors[0][i].a);
		}

		if (aiMesh->mNumBones >= 1)
		{
			meshData.vertexData[index++] = 0.0f;
			meshData.vertexData[index++] = 0.0f;
			meshData.vertexData[index++] = 0.0f;
			meshData.vertexData[index++] = 0.0f;

			meshData.vertexData[index++] = -1.1f;
			meshData.vertexData[index++] = -1.1f;
			meshData.vertexData[index++] = -1.1f;
			meshData.vertexData[index++] = -1.1f;
		}

	}

	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshData.indexData.push_back(face.mIndices[j]);
	}
	
	ParseBoneWeight(aiMesh, scene, meshData);

	////材质对象的内部对每种纹理类型都存储了一个纹理位置数组。不同的纹理类型都以aiTextureType_为前缀
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//return Mesh(vertices, indices, textures);

}

void MeshLoader::ParseBoneWeight(aiMesh* mesh, const aiScene* scene, Mesh::Data& meshData)
{
	auto& boneInfoMap = _boneInfoMap;
	int& boneCount = _boneCounter;

	for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.index = boneCount;
			newBoneInfo.matrix = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].index;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;

			SetVertexBoneData(meshData, vertexId, boneID + 0.1f, weight);
		}
	}

}

void MeshLoader::SetVertexBoneData(Mesh::Data& meshData, int vertexId, float boneID, float weight)
{
	int elementCount = meshData.vertexStride / 4;
	int index = elementCount * vertexId;
	int weightIndex = elementCount - 8;
	int boneIndex = elementCount - 4;

	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		weightIndex += i;
		boneIndex += i;
		if (meshData.vertexData[index + boneIndex] < 0)
		{
			meshData.vertexData[index + boneIndex] = boneID;
			meshData.vertexData[index + weightIndex] = weight;
			break;
		}
	}
}
