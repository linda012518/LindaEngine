#include "FBXLoader.h"
#include "Entity.h"
#include "AssimpGLMHelpers.h"
#include "Mesh.h"
#include "Path.h"
#include "AnimationClip.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

using namespace LindaEngine;

Ref<FBXResources> FBXLoader::LoadFBX(std::string path)
{
	Assimp::Importer importer;
	//aiProcess_Triangulate			如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
	//aiProcess_FlipUVs				在处理的时候翻转y轴的纹理坐标
	//aiProcess_GenNormals			如果模型不包含法向量的话，就为每个顶点创建法线
	//aiProcess_SplitLargeMeshes	将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用
	//aiProcess_OptimizeMeshes		和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData);

	//mFlags	返回的数据是不是不完整的
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	Ref<AssimpNodeData> dataPointer = CreateRef<AssimpNodeData>();
	AssimpNodeData& data = *dataPointer.get();
	int index = 0;
	ParseAssimpFBX(nullptr, data, scene->mRootNode, scene, index);
	SetHashBoneInfoMap(data, data);

	Ref<FBXResources> res = CreateRef<FBXResources>();
	ConvertFBXResources(res, data, path);
	res->name = Path::GetFileNameNoExtension(path);

	return res;
}

Ref<AnimationClip> FBXLoader::LoadAnimationClip(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_PopulateArmatureData);

	//mFlags	返回的数据是不是不完整的
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	return ParseAnimationClip(scene);
}

void FBXLoader::ParseAssimpFBX(void* mat, AssimpNodeData& dest, aiNode* node, const aiScene* scene, int& index)
{
	aiMatrix4x4 nodeTransformation = node->mTransformation;
	if (nullptr != mat)
		nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(mat) * node->mTransformation;
	index++;

	std::string tempName = node->mName.data;
	if (tempName.find("_$AssimpFbx$_") != std::string::npos)
	{
		ParseAssimpFBX(&nodeTransformation, dest, node->mChildren[0], scene, index);
	}
	else
	{
		dest.name = tempName;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(nodeTransformation);
		dest.childrenCount = node->mNumChildren;
		dest.nativeNode = node;

		std::hash<std::string> hasher;
		std::string path = std::to_string(index);
		ParseAssimpNodePath(node, path);
		std::stringstream hexStream;
		hexStream << std::hex << std::setfill('0') << std::setw(16) << hasher(path);
		dest.hashCode = hexStream.str();

		if (node->mNumMeshes > 0)
			dest.hasMesh = true;

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			ParseAssimpMesh(aiMesh, dest);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ParseAssimpFBX(nullptr, newData, node->mChildren[i], scene, index);
			dest.children.push_back(newData);
		}
	}

}

void FBXLoader::ParseAssimpMesh(aiMesh* aiMesh, AssimpNodeData& node)
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

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		Vertex vertex;

		SetVertexBoneDataToDefault(vertex);

		// Keep vertices in mesh local/bind-pose space.
		// Node transforms are represented by the scene graph (Entity/Transform) at runtime.
		vertex.position = AssimpGLMHelpers::GetGLMVec(aiMesh->mVertices[i]);

		if (NULL != aiMesh->mNormals)
			vertex.normal = AssimpGLMHelpers::GetGLMVec(aiMesh->mNormals[i]);
		if (NULL != aiMesh->mTangents)
			vertex.tangent = AssimpGLMHelpers::GetGLMVec(aiMesh->mTangents[i]);

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

	FBXMesh& mesh = node.mesh;
	if (aiMesh->mNumBones > 0)
	{
		ExtractBoneWeightForVertices(vertices, aiMesh, mesh);
		mesh.isSkinMesh = true;
	}
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

void FBXLoader::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, FBXMesh& fbxMesh)
{
	int& boneCount = fbxMesh.boneCount;
	std::vector<BoneInfo>& boneArray = fbxMesh.bones;

	for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		aiBone* bone = mesh->mBones[boneIndex];
		int boneID = -1;

		for (auto& go : boneArray)
		{
			if (go.nativeNode != bone->mNode)
				continue;
			boneID = go.id;
		}

		if (boneID == -1)
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(bone->mOffsetMatrix);
			newBoneInfo.name = bone->mName.data;
			newBoneInfo.nativeNode = bone->mNode;
			boneArray.push_back(newBoneInfo);
			boneID = boneCount;
			boneCount++;
		}
		assert(boneID != -1);
		auto weights = bone->mWeights;
		int numWeights = bone->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void FBXLoader::SetHashBoneInfoMap(AssimpNodeData& root, AssimpNodeData& nodeData)
{
	if (nodeData.hasMesh && nodeData.mesh.isSkinMesh)
	{
		std::vector<BoneInfo>& temp = nodeData.mesh.bones;
		for (auto& go : temp)
		{
			go.hashCode = GetNodeByNativeNode(root, go.nativeNode);
		}
	}
	for (auto& go : nodeData.children)
	{
		SetHashBoneInfoMap(root, go);
	}
}

std::string FBXLoader::GetNodeByNativeNode(AssimpNodeData& root, aiNode* nativeNode)
{
	if (root.nativeNode == nativeNode)
		return root.hashCode;

	for (auto& go : root.children)
	{
		std::string temp = GetNodeByNativeNode(go, nativeNode);
		if (temp != "") return temp;
	}

	return "";
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

void FBXLoader::ParseAssimpNodePath(aiNode* node, std::string& path)
{
	if (node == nullptr)
		return;
	path += "/";
	path += node->mName.data;
	ParseAssimpNodePath(node->mParent, path);
}

void FBXLoader::ConvertFBXResources(Ref<FBXResources> res, AssimpNodeData& data, std::string& path)
{
	glm::vec3 skew;
	glm::vec4 m;
	glm::decompose(data.transformation, res->localScale, res->localRotation, res->localPosition, skew, m);

	res->hashCode = data.hashCode;
	res->name = data.name;
	if (data.hasMesh)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetHashCode(data.hashCode);
		mesh->SetPath(path);
		for (auto& go : data.mesh.vertices)
		{
			Mesh::Data& meshData = mesh->AddMeshData(Mesh::Data());
			for (auto& attr : go.attributes)
			{
				meshData.AddAttribute(attr);
			}
			int index = 0;
			meshData.vertexData.resize(((size_t)meshData.vertexStride / 4) * ((size_t)go.vertices.size()));
			for (auto& vertex : go.vertices)
			{
				meshData.vertexData[index++] = vertex.position.x;
				meshData.vertexData[index++] = vertex.position.y;
				meshData.vertexData[index++] = vertex.position.z;
				mesh->UpdateBoundingBox(vertex.position.x, vertex.position.y, vertex.position.z);

				if (HasAttribute(go.attributes, VertexAttributeType::Normal))
				{
					meshData.vertexData[index++] = vertex.normal.x;
					meshData.vertexData[index++] = vertex.normal.y;
					meshData.vertexData[index++] = vertex.normal.z;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::Tangent))
				{
					meshData.vertexData[index++] = vertex.tangent.x;
					meshData.vertexData[index++] = vertex.tangent.y;
					meshData.vertexData[index++] = vertex.tangent.z;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV0))
				{
					meshData.vertexData[index++] = vertex.texCoord0.x;
					meshData.vertexData[index++] = vertex.texCoord0.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV1))
				{
					meshData.vertexData[index++] = vertex.texCoord1.x;
					meshData.vertexData[index++] = vertex.texCoord1.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV2))
				{
					meshData.vertexData[index++] = vertex.texCoord2.x;
					meshData.vertexData[index++] = vertex.texCoord2.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV3))
				{
					meshData.vertexData[index++] = vertex.texCoord3.x;
					meshData.vertexData[index++] = vertex.texCoord3.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV4))
				{
					meshData.vertexData[index++] = vertex.texCoord4.x;
					meshData.vertexData[index++] = vertex.texCoord4.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV5))
				{
					meshData.vertexData[index++] = vertex.texCoord5.x;
					meshData.vertexData[index++] = vertex.texCoord5.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV6))
				{
					meshData.vertexData[index++] = vertex.texCoord6.x;
					meshData.vertexData[index++] = vertex.texCoord6.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::UV7))
				{
					meshData.vertexData[index++] = vertex.texCoord7.x;
					meshData.vertexData[index++] = vertex.texCoord7.y;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::Color))
				{
					meshData.vertexData[index++] = vertex.color.r;
					meshData.vertexData[index++] = vertex.color.g;
					meshData.vertexData[index++] = vertex.color.b;
					meshData.vertexData[index++] = vertex.color.a;
				}

				if (HasAttribute(go.attributes, VertexAttributeType::BoneID1))
				{
					meshData.vertexData[index++] = (float)vertex.boneID[0];
					meshData.vertexData[index++] = (float)vertex.boneID[1];
					meshData.vertexData[index++] = (float)vertex.boneID[2];
					meshData.vertexData[index++] = (float)vertex.boneID[3];

					meshData.vertexData[index++] = vertex.boneWeight[0];
					meshData.vertexData[index++] = vertex.boneWeight[1];
					meshData.vertexData[index++] = vertex.boneWeight[2];
					meshData.vertexData[index++] = vertex.boneWeight[3];
				}
			}
			
			meshData.indexData = go.indices;
		}
		res->mesh = mesh;

		res->boneCount = data.mesh.boneCount;

		for (auto& go : data.mesh.bones)
		{
			BoneData bone;
			bone.id = go.id;
			bone.name = go.name;
			bone.hashCode = go.hashCode;
			bone.offset = go.offset;
			res->bones.push_back(bone);
		}
	}

	for (auto& go : data.children)
	{
		Ref<FBXResources> child = CreateRef<FBXResources>();
		ConvertFBXResources(child, go, path);
		res->children.push_back(child);
	}
}

bool FBXLoader::HasAttribute(std::vector<VertexAttributeType>& attributes, VertexAttributeType attr)
{
	auto itr = std::find(attributes.begin(), attributes.end(), attr);
	if (itr != attributes.end())
		return true;
	return false;
}

Ref<AnimationClip> FBXLoader::ParseAnimationClip(const aiScene* scene)
{
	if (nullptr == scene || scene->mNumAnimations <= 0)
		return nullptr;

	aiAnimation* animation = scene->mAnimations[0];

	int size = animation->mNumChannels;

	Ref<AnimationClip> clip = CreateRef<AnimationClip>();
	clip->duration = (float)animation->mDuration * 0.001f;
	clip->ticksPerSecond = (float)animation->mTicksPerSecond * 0.001f;
	clip->name = animation->mName.C_Str();

	for (int i = 0; i < size; i++)
	{
		aiNodeAnim* channel = animation->mChannels[i];
		BoneTrack track;
		track.name = channel->mNodeName.C_Str();

		int numPositions = channel->mNumPositionKeys;
		for (int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
			AnimationPosePosition data;
			data.data = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
			data.timeStamp = timeStamp * 0.001f;
			track.tracksPosition.push_back(data);
		}

		int numRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
			AnimationPoseRotation data;
			data.data = glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
			data.timeStamp = timeStamp * 0.001f;
			track.tracksRotation.push_back(data);
		}

		int numScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < numScalings; ++keyIndex)
		{
			aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
			float timeStamp = (float)channel->mScalingKeys[keyIndex].mTime;
			AnimationPoseScale data;
			data.data = glm::vec3(scale.x, scale.y, scale.z);
			data.timeStamp = timeStamp * 0.001f;
			track.tracksScale.push_back(data);
		}

		clip->tracks.push_back(track);
	}

	return clip;
}
