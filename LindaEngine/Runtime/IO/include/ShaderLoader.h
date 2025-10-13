#pragma once

#include <string>
#include <vector>

namespace LindaEngine
{
	struct ShaderSource;

	class ShaderLoader
	{
	public:
		static std::vector<ShaderSource> Load(const char* path);

	private:
		static void GetFilePaths(const char* path, std::vector<std::string>& paths);

		static void ProcessInclude(std::string& tex, std::vector<std::string>& paths);

		static void DeleteShaderFrame(std::string& tex);

		static std::string GetPassName(std::string& tex);

		static std::vector<ShaderSource> GetPasses(std::string url, std::string& tex);

		static void CollectRemoveAttributes(std::string& tex);
		static void CollectUniforms(std::string& tex);
	};
}
