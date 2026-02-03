#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "AutoPtr.h"
#include "RenderEnumData.h"

namespace LindaEngine
{
	struct ShaderSource;
	struct ShaderSourceCode;

	class ShaderLoader
	{
	public:
		static Ref<ShaderSource> Load(const char* path);
		static void DeleteShaderFrame(std::string& tex);
		static void ProcessInclude(std::string& tex, std::vector<std::string>& paths);
		static void AddPickOut(std::string& str, std::string& shaderCode);

	private:
		static RenderType GetRenderType(std::string& tex);
		static int GetRenderQueue(std::string& tex);
		static bool HasFallback(std::string& tex);

		static void GetPasses(std::string& tex, Ref<ShaderSource> ss);
		static void GetPassName(std::string& tex, Ref<ShaderSourceCode> passState);
		static void CollectProperties(std::string& tex, Ref<ShaderSourceCode> passState);
		static void CollectKeywords(std::string& tex, Ref<ShaderSourceCode> passState);
		static void CollectRenderState(std::string& tex, Ref<ShaderSourceCode> passState);
		static void CollectShaderCode(std::string& tex, Ref<ShaderSourceCode> passState);

		static void ParseColorMask(std::string& tex, Ref<ShaderSourceCode> passState);
		static void ParseDepthState(std::string& tex, Ref<ShaderSourceCode> passState);
		static void ParseCullFaceFunc(std::string& tex, Ref<ShaderSourceCode> passState);
		static void ParseStencilState(std::string& tex, Ref<ShaderSourceCode> passState);
		static void ParseBlendState(std::string& tex, Ref<ShaderSourceCode> passState);
		static void ParsePolygonModeState(std::string& tex, Ref<ShaderSourceCode> passState);


		static std::string ExtractContent(std::string& tex, std::string mark, char front, char back, bool deleteContent = true);

	};
}
