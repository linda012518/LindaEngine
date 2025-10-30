#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <string>
#include <unordered_map>

namespace LindaEngine
{
	struct ShaderUniform; 
	struct RenderState;

	struct MaterialPassState
	{
		std::unordered_map<std::string, Ref<ShaderUniform>> uniformNameMap;
		std::vector<std::string> keywords;
		std::string lightMode;
		Ref<RenderState> renderState;
	};

	struct MaterialState
	{
		bool isError;
		bool hasFallback;
		int renderQueue;
		RenderType renderType;
		std::string shaderPath;
		std::string materialPath;
	};
}
