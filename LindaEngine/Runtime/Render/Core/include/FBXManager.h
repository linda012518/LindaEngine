#pragma once

#include "AutoPtr.h"
#include "Mesh.h"
#include "FBXLoader.h"

#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class FBXManager
	{
	public:
		static Ref<FBXResources> GetFBX(std::string fbxPath);
		static Ref<Mesh> GetMesh(std::string fbxPath, std::string hashCode);
		static std::vector<BoneData>& GetMeshBoneData(std::string fbxPath, std::string hashCode);
		static void DestoryFBX(std::string fbxPath);
		static void Clear();

		static Ref<Mesh> GetSkybox();
		static Ref<Mesh> GetEmpty();

		static Ref<Mesh> GetBoundingBox();
		static Ref<Mesh> GetFrustumMesh();
		static Ref<Mesh> GetLightMesh();

	private:
		static Ref<FBXResources> GetFBXResources(Ref<FBXResources> res, std::string hashCode);
		static void ClearFBX(Ref<FBXResources> res);
		static Ref<Mesh> CreateCubeMesh(DrawType type, float scale);

	private:
		static std::unordered_map<std::string, Ref<FBXResources>> _map;
		static Ref<Mesh> _skybox;
		static Ref<Mesh> _empty;
		static Ref<Mesh> _boundingBox;
		static Ref<Mesh> _frustumMesh;
		
	};
}
