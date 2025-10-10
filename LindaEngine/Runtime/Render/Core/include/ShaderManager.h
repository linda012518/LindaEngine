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
	};

	class ShaderManager
	{

	public:
		static std::vector<ShaderSource>& GetShaderSource(std::string& path);
		static std::string& GetInclude(std::string& path);

	private:
		
	private:
		static std::unordered_map<std::string, std::vector<ShaderSource>> _shaderSrcMap;
		static std::unordered_map<std::string, std::string> _includes;
	};
}
