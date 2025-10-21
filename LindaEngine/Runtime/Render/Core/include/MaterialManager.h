#pragma once

#include <unordered_map>
#include <string>

#include "AutoPtr.h"

namespace LindaEngine
{
	class Material;

	class MaterialManager
	{
	public:
		static Ref<Material> GetMaterial(const char* path);
		static void Clear();

	private:
		static std::unordered_map<std::string, Ref<Material>> _materialMap;
	};
}
