#pragma once

#include "AutoPtr.h"
#include "Mesh.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace LindaEngine
{
	class Entity;

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
	};

	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	struct FBXModel
	{
		std::vector<FBXMesh> meshs;
		AssimpNodeData hierarchy;
	};

	struct BoneInfo
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;

	};

	class FBXLoader
	{
	public:
		std::vector<Ref<Entity>> LoadFBX(std::string path);

	private:
		void ParseAssimpFBX(void* mat, AssimpNodeData& dest, aiNode* node, const aiScene* scene, FBXModel& model);
		void ParseAssimpMesh(void* mat, aiMesh* aiMesh, const aiScene* scene, FBXModel& model);
		void FillVertexUV(glm::vec2& dest, int uvIndex, int vertexIndex, aiMesh* aiMesh);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
		void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
		void SetVertexBoneDataToDefault(Vertex& vertex);
	};
}
