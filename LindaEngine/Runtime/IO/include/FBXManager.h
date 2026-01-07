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
		static void DestoryFBX(std::string fbxPath);
		static void Clear();

		static Ref<Mesh> LoadSkybox();
		static Ref<Mesh> LoadEmpty();

	private:
		static Ref<Mesh> GetMeshFromFBX(Ref<FBXResources> res, std::string hashCode);

	private:
		static std::unordered_map<std::string, Ref<FBXResources>> _map;
	};
}
