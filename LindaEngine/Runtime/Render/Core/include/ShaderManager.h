#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "AutoPtr.h"
#include "MaterialState.h"

namespace LindaEngine
{
	struct ShaderSourceCode
	{
		std::string properties;
		std::string vertex;
		std::string fragment;
		MaterialPassState passState;
	};

	struct ShaderSource
	{
		MaterialState state;
		std::vector<Ref<ShaderSourceCode>> shaderSrcCode;
	};

	class Shader;
	struct VertexAttribute;

	class ShaderManager
	{
	public:
		static Ref<ShaderSource>& GetShaderSource(const char* path);
		static std::string& GetInclude(const char* path);
		static void Clear();
		static Ref<Shader> CompileShader(Ref<ShaderSourceCode> sss, std::vector<std::string>& keywords, const std::vector<VertexAttribute>& attributes);
	
	private:
		static bool Initialize();

	public:
		static std::vector<std::string> defaultAttributeNames;
		static std::string defaultShaderUniformBlack;
		static std::string defaultShaderVersion;
		static std::string defaultShaderUniform;
		
	private:
		static std::unordered_map<std::string, Ref<ShaderSource>> _shaderSrcMap;
		static std::unordered_map<std::string, std::string> _includes;
	};
}
