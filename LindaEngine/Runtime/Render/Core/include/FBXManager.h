#pragma once

#include "AutoPtr.h"
#include "Mesh.h"
#include "FBXLoader.h"

#include <string>
#include <unordered_map>

namespace LindaEngine
{
	enum class LightType;

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
		static Ref<Mesh> GetLightMesh(LightType type);

	private:
		static Ref<FBXResources> GetFBXResources(Ref<FBXResources> res, std::string hashCode);
		static void ClearFBX(Ref<FBXResources> res);
		static Ref<Mesh> CreateCubeMesh(DrawType type, float scale);
		static Ref<Mesh> CreateDirectionLightMesh();
		static Ref<Mesh> CreatePointLightMesh();
		static Ref<Mesh> CreateSpotLightMesh();

	private:
		static std::unordered_map<std::string, Ref<FBXResources>> _map;
		static Ref<Mesh> _skybox;
		static Ref<Mesh> _empty;
		static Ref<Mesh> _boundingBox;
		static Ref<Mesh> _frustumMesh;
		static Ref<Mesh> _directionLightMesh;
		static Ref<Mesh> _pointLightMesh;
		
	};
}
