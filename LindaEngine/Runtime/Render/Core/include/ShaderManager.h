#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "AutoPtr.h"
#include "ShaderUniform.h"
#include "Material.h"

namespace LindaEngine
{
	struct ShaderSourceCode
	{
		std::string name;
		std::unordered_map<std::string, UniformType> uniformsNameMapType;
		//RenderState
		std::vector<std::string> keywords;
		std::string vertex;
		std::string fragment;
		std::vector<std::string> attributeNames;
	};

	struct ShaderSource
	{
		bool hasFallback = false;
		int renderQueue = 2000;
		RenderType renderType = RenderType::Opaque;
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

	public:
		static std::vector<std::string> defaultAttributeNames;
		static std::string defaultShaderUniformBlack;
		static std::string defaultShaderVersion;
		
	private:
		static std::unordered_map<std::string, Ref<ShaderSource>> _shaderSrcMap;
		static std::unordered_map<std::string, std::string> _includes;
	};
}
