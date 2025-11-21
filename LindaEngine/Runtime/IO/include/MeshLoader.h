#pragma once

#include "AutoPtr.h"
#include "Mesh.h"

#include <string>
#include <unordered_map>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace LindaEngine
{
	struct BoneInfo
	{
		int index;
		glm::mat4 matrix;
	};

	class MeshLoader
	{
	public:
		static Ref<Mesh> Load(std::string path);

	private:
		static Ref<Mesh> LoadSphereMesh();
		static Ref<Mesh> LoadCube();
		static Ref<Mesh> LoadCapsule();
		static Ref<Mesh> LoadPlane();
		static Ref<Mesh> LoadSkybox();
		static Ref<Mesh> LoadQuad();
		static Ref<Mesh> LoadEmpty();

	private:
		static void ParseAssimpNode(aiNode* node, const aiScene* scene, Ref<Mesh> mesh);
		static void ParseAssimpMesh(aiMesh* aiMesh, const aiScene* scene, Mesh::Data& meshData, Ref<Mesh> meshPtr);
		static void ParseBoneWeight(aiMesh* mesh, const aiScene* scene, Mesh::Data& meshData);
		static void SetVertexBoneData(Mesh::Data& meshData, int vertexId, float boneID, float weight);

	private:
		static std::unordered_map<std::string, BoneInfo> _boneInfoMap;
		static int _boneCounter;

	};
}
