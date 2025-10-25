#pragma once

#include <unordered_map>
#include <string>

#include "AutoPtr.h"

namespace LindaEngine
{
	class Material;
	class MaterialPass;

	class MaterialManager
	{
	public:
		static Ref<Material> GetMaterial(const char* path);
		static void Clear();
		static Ref<MaterialPass> GetDefaultMaterialPass(const char* lightMode);
		static Ref<Material> GetDefaultMaterial(const char* path);

	private:
		static bool LoadDefaultMaterial();

	private:
		static std::unordered_map<std::string, Ref<Material>> _materialMap;
		static std::unordered_map<std::string, Ref<Material>> _defaultMaterial;
		static std::unordered_map<std::string, Ref<MaterialPass>> _defaultPass;
		static bool _isLoadDefault;
	};
}
