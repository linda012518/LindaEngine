#pragma once

#include <string>
#include <vector>

namespace LindaEngine
{
	class Path
	{
	public:
		static std::vector<std::string> GetFileDirtcorys(const char* path);
		static std::string GetFilePath(std::string& path);
		static std::string GetFileName(std::string& path);

		static const char* overridePath;
		static const char* graphicsConfig;
		static const char* sceneConfig;
		static const char* shaderConfig;
	};
}
