#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "AutoPtr.h"

namespace LindaEngine
{
	struct ShaderSource;
	struct ShaderSourceCode;
	enum class UniformType;

	class ShaderLoader
	{
	public:
		static Ref<ShaderSource> Load(const char* path);

	private:
		static void GetFilePaths(const char* path, std::vector<std::string>& paths);

		static void ProcessInclude(std::string& tex, std::vector<std::string>& paths);

		static void DeleteShaderFrame(std::string& tex);

		static bool HasFallback(std::string& tex);

		static std::string GetPassName(std::string& tex);

		static void GetPasses(std::string url, std::string& tex);

		static void CollectAttributes(Ref<ShaderSourceCode> ss);
		static void CollectUniforms(Ref<ShaderSourceCode> ss, std::string& tex);
		static std::string GetAttrDataTypeByName(std::string& name, int& index);
		static UniformType GetUniformDataTypeByName(std::string& name);
	};
}
