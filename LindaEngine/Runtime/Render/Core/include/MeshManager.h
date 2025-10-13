#pragma once

#include <unordered_map>
#include <string>

#include "AutoPtr.h"

namespace LindaEngine
{
	class Mesh;
	class MeshManager
	{
	public:
		static Ref<Mesh> GetMesh(const char* path);

		static Ref<Mesh> GetSphereMesh();
		static Ref<Mesh> GetCube();
		static Ref<Mesh> GetCapsule();
		static Ref<Mesh> GetPlane();
		static Ref<Mesh> GetSkybox();

	private:
		static std::unordered_map<std::string, Ref<Mesh>> _meshMap;
	};
}
