#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace LindaEngine
{
	struct ShaderSource
	{
		std::string name;
		std::string vertex;
		std::string fragment;
		std::vector<std::string> attributeNames;
		std::unordered_map<std::string, std::string> uniformsNameMapType;
	};

	class ShaderManager
	{

	public:
		static std::vector<ShaderSource>& GetShaderSource(const char* path);
		static std::string& GetInclude(const char* path);

	public:
		static std::vector<std::string> defaultAttributeNames;
		
	private:
		static std::unordered_map<std::string, std::vector<ShaderSource>> _shaderSrcMap;
		static std::unordered_map<std::string, std::string> _includes;
	};
}
