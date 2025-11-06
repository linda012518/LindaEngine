#pragma once

#include "AutoPtr.h"
#include "Mesh.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace LindaEngine
{
	class MeshLoader
	{
	public:
		static Ref<Mesh> Load(std::string path);

		static Ref<Mesh> LoadSphereMesh();
		static Ref<Mesh> LoadCube();
		static Ref<Mesh> LoadCapsule();
		static Ref<Mesh> LoadPlane();
		static Ref<Mesh> LoadSkybox();

	private:
		static void ParseAssimpNode(aiNode* node, const aiScene* scene, Ref<Mesh> mesh);
		static void ParseAssimpMesh(aiMesh* aiMesh, const aiScene* scene, Mesh::Data& meshData, Ref<Mesh> meshPtr);
	};
}
