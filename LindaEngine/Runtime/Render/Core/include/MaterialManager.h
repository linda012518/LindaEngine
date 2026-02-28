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
		static Ref<Material> GetMaterial(std::string path, bool isSkin = false);
		static Ref<Material> GetMaterialByShader(std::string path, bool isSkin = false); //需要使用者管理内存
		static void Clear();
		static Ref<MaterialPass> GetDefaultMaterialPass(const char* lightMode);
		static Ref<Material> GetDefaultMaterial(std::string path, bool isSkin = false);

	private:
		static bool LoadDefaultMaterial();

	private:
		static std::unordered_map<std::string, Ref<Material>> _materialMap;
		static std::unordered_map<std::string, Ref<Material>> _defaultMaterial;
		static std::unordered_map<std::string, Ref<MaterialPass>> _defaultPass;
		static bool _isLoadDefault;
	};
}
