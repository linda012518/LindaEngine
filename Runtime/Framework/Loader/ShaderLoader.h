#pragma once

#include <string>
#include <vector>

namespace LindaEngine
{
	class ShaderLoader
	{
	public:
		static std::string Load(const char* path);

	private:
		static void GetFilePaths(const char* path, std::vector<std::string>& paths);

		static void ProcessInclude(std::string& tex, std::vector<std::string>& paths);

		static void DeleteShaderFrame(std::string& tex);

		static void GetPasses(std::string url, std::string& tex);
	};
}
