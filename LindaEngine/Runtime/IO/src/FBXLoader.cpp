#include "FBXLoader.h"
#include "Entity.h"
#include "AssimpGLMHelpers.h"
#include "Mesh.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <iostream>
#include <map>

using namespace LindaEngine;

std::vector<Ref<Entity>> FBXLoader::LoadFBX(std::string path)
{
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
		return std::vector<Ref<Entity>>();
	}

	FBXModel model;
	ParseAssimpFBX(nullptr, model.hierarchy, scene->mRootNode, scene, model);

	return std::vector<Ref<Entity>>();
}

void FBXLoader::ParseAssimpFBX(void* mat, AssimpNodeData& dest, aiNode* node, const aiScene* scene, FBXModel& model)
{
	aiMatrix4x4 nodeTransformation = node->mTransformation;
	if (nullptr != mat)
		nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(mat) * node->mTransformation;

	std::string tempName = node->mName.data;
	if (tempName.find("_$AssimpFbx$_") != std::string::npos)
	{
		ParseAssimpFBX(&nodeTransformation, dest, node->mChildren[0], scene, model);
	}
	else
	{
		dest.name = tempName;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(nodeTransformation);
		dest.childrenCount = node->mNumChildren;

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			ParseAssimpMesh(&nodeTransformation, aiMesh, scene, model);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			aiNode* temp = node->mChildren[i];
			if (temp->mNumMeshes > 0)
				model.meshs.push_back(FBXMesh());
			AssimpNodeData newData;
			ParseAssimpFBX(nullptr, newData, node->mChildren[i], scene, model);
			dest.children.push_back(newData);
		}
	}

}

void FBXLoader::ParseAssimpMesh(void* mat, aiMesh* aiMesh, const aiScene* scene, FBXModel& model)
{
	if (aiMesh->mNumVertices < 3)
		return;

	FBXMeshData meshData;
	std::vector<Vertex>& vertices = meshData.vertices;
	std::vector<unsigned int>& indices = meshData.indices;

	meshData.attributes.push_back(VertexAttributeType::Position);
	if (NULL != aiMesh->mNormals) meshData.attributes.push_back(VertexAttributeType::Normal);
	if (NULL != aiMesh->mTangents) meshData.attributes.push_back(VertexAttributeType::Tangent);
	if (NULL != aiMesh->mTextureCoords[0]) meshData.attributes.push_back(VertexAttributeType::UV0);
	if (NULL != aiMesh->mTextureCoords[1]) meshData.attributes.push_back(VertexAttributeType::UV1);
	if (NULL != aiMesh->mTextureCoords[2]) meshData.attributes.push_back(VertexAttributeType::UV2);
	if (NULL != aiMesh->mTextureCoords[3]) meshData.attributes.push_back(VertexAttributeType::UV3);
	if (NULL != aiMesh->mTextureCoords[4]) meshData.attributes.push_back(VertexAttributeType::UV4);
	if (NULL != aiMesh->mTextureCoords[5]) meshData.attributes.push_back(VertexAttributeType::UV5);
	if (NULL != aiMesh->mTextureCoords[6]) meshData.attributes.push_back(VertexAttributeType::UV6);
	if (NULL != aiMesh->mTextureCoords[7]) meshData.attributes.push_back(VertexAttributeType::UV7);
	if (NULL != aiMesh->mColors[0]) meshData.attributes.push_back(VertexAttributeType::Color);
	if (NULL != aiMesh->mBones && aiMesh->mNumBones > 0)
	{
		meshData.attributes.push_back(VertexAttributeType::BoneID1);
		meshData.attributes.push_back(VertexAttributeType::BoneWeights1);
	}

	aiMatrix4x4 meshTransformation;
	if (nullptr != mat)
		meshTransformation = *reinterpret_cast<aiMatrix4x4*>(mat);

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		Vertex vertex;

		SetVertexBoneDataToDefault(vertex);

		vertex.position = AssimpGLMHelpers::GetGLMVec(meshTransformation * aiMesh->mVertices[i]);

		if (NULL != aiMesh->mNormals)
			vertex.normal = AssimpGLMHelpers::GetGLMVec(meshTransformation * aiMesh->mNormals[i]);
		if (NULL != aiMesh->mTangents)
			vertex.tangent = AssimpGLMHelpers::GetGLMVec(meshTransformation * aiMesh->mTangents[i]);

		FillVertexUV(vertex.texCoord0, 0, i, aiMesh);
		FillVertexUV(vertex.texCoord1, 1, i, aiMesh);
		FillVertexUV(vertex.texCoord2, 2, i, aiMesh);
		FillVertexUV(vertex.texCoord3, 3, i, aiMesh);
		FillVertexUV(vertex.texCoord4, 4, i, aiMesh);
		FillVertexUV(vertex.texCoord5, 5, i, aiMesh);
		FillVertexUV(vertex.texCoord6, 6, i, aiMesh);
		FillVertexUV(vertex.texCoord7, 7, i, aiMesh);

		if (NULL != aiMesh->mColors[0])
		{
			vertex.color.r = aiMesh->mColors[0][i].r;
			vertex.color.g = aiMesh->mColors[0][i].g;
			vertex.color.b = aiMesh->mColors[0][i].b;
			vertex.color.a = aiMesh->mColors[0][i].a;
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	ExtractBoneWeightForVertices(vertices, aiMesh, scene);

	FBXMesh& mesh = model.meshs[model.meshs.size() - 1];
	mesh.vertices.push_back(meshData);
}

void FBXLoader::FillVertexUV(glm::vec2& dest, int uvIndex, int vertexIndex, aiMesh* aiMesh)
{
	if (NULL != aiMesh->mTextureCoords[uvIndex])
	{
		glm::vec2 vec;
		vec.x = aiMesh->mTextureCoords[uvIndex][vertexIndex].x;
		vec.y = aiMesh->mTextureCoords[uvIndex][vertexIndex].y;
		dest = vec;
	}
}

void FBXLoader::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	std::map<std::string, BoneInfo> boneInfoMap;
	int boneCount = 0;

	for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}

}

void FBXLoader::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.boneID[i] < 0)
		{
			vertex.boneWeight[i] = weight;
			vertex.boneID[i] = boneID;
			break;
		}
	}
}

void FBXLoader::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneID[i] = -1;
		vertex.boneWeight[i] = 0.0f;
	}
}
