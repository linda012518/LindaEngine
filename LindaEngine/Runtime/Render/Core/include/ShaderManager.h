#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "AutoPtr.h"
#include "ShaderUniform.h"

namespace LindaEngine
{
	struct ShaderSourceCode
	{
		std::string name;
		std::string vertex;
		std::string fragment;
		std::vector<std::string> attributeNames;
		std::unordered_map<std::string, UniformType> uniformsNameMapType;
	};

	struct ShaderSource
	{
		bool hasFallback = false;
		std::vector<Ref<ShaderSourceCode>> shaderSrcCode;
	};

	class Shader;

	class ShaderManager
	{
	public:
		static Ref<ShaderSource>& GetShaderSource(const char* path);
		static std::string& GetInclude(const char* path);
		static void Clear();

	public:
		static std::vector<std::string> defaultAttributeNames;
		static std::string defaultShaderUniformBlack;
		static std::string defaultShaderVersion;
		
	private:
		static std::unordered_map<std::string, Ref<ShaderSource>> _shaderSrcMap;
		static std::unordered_map<std::string, std::string> _includes;
	};
}
