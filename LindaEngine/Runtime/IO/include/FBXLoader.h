#pragma once

#include "AutoPtr.h"
#include "Mesh.h"

#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <functional>
#include <sstream>
#include <iomanip>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace LindaEngine
{
	class Entity;

	struct BoneInfo
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;

		/*fbx node hashCode*/
		std::string hashCode;

		std::string name;

		aiNode* nativeNode;
	};

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;

		glm::vec2 texCoord0;
		glm::vec2 texCoord1;
		glm::vec2 texCoord2;
		glm::vec2 texCoord3;
		glm::vec2 texCoord4;
		glm::vec2 texCoord5;
		glm::vec2 texCoord6;
		glm::vec2 texCoord7;

		glm::vec4 color;

		int boneID[MAX_BONE_INFLUENCE];
		float boneWeight[MAX_BONE_INFLUENCE];
	};

	struct FBXMeshData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<VertexAttributeType> attributes;
	};

	struct FBXMesh
	{
		std::vector<FBXMeshData> vertices;
		bool isSkinMesh = false;
		std::vector<BoneInfo> bones;
		int boneCount = 0;
	};

	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount = 0;
		std::vector<AssimpNodeData> children;
		FBXMesh mesh;
		bool hasMesh = false;
		std::string hashCode;
		aiNode* nativeNode;
	};

	struct BoneData
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;

		/*fbx node hashCode*/
		std::string hashCode;

		std::string name;
	};

	struct FBXResources
	{
		glm::vec3 localPosition;
		glm::quat localRotation;
		glm::vec3 localScale;
		std::string name;
		std::vector<Ref<FBXResources>> children;
		Ref<Mesh> mesh = nullptr;
		std::string hashCode;

		std::vector<BoneData> bones;
		int boneCount = 0;
	};

	class FBXLoader
	{
	public:
		static Ref<FBXResources> LoadFBX(std::string path);

	private:
		static void ParseAssimpFBX(void* mat, AssimpNodeData& dest, aiNode* node, const aiScene* scene, int& index);
		static void ParseAssimpMesh(void* mat, aiMesh* aiMesh, const aiScene* scene, AssimpNodeData& node);
		static void FillVertexUV(glm::vec2& dest, int uvIndex, int vertexIndex, aiMesh* aiMesh);
		static void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene, FBXMesh& fbxMesh);
		static void SetHashBoneInfoMap(AssimpNodeData& root, AssimpNodeData& nodeData);
		static std::string GetNodeByNativeNode(AssimpNodeData& root, aiNode* nativeNode);
		static void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
		static void SetVertexBoneDataToDefault(Vertex& vertex);
		static void ParseAssimpNodePath(aiNode* node, std::string& path);

		static void ConvertFBXResources(Ref<FBXResources> res, AssimpNodeData& data, std::string& path);
		static bool HasAttribute(std::vector<VertexAttributeType>& attributes, VertexAttributeType attr);

		static void ParseAnimationClip(const aiScene* scene);
	};
}
