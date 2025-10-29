#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace LindaEngine { struct ShaderUniform; struct RenderState; }

namespace LindaEditor
{
	struct MaterialPassStateEditor
	{
		std::unordered_map<std::string, Ref<LindaEngine::ShaderUniform>> uniformNameMap;
		std::vector<std::string> keywords;
		std::string lightMode;
		Ref<LindaEngine::RenderState> renderState;
	};

	struct MaterialStateEditor
	{
		bool isError;
		bool hasFallback;
		int renderQueue;
		LindaEngine::RenderType renderType;
		std::string shaderPath;
		std::string materialPath;
		std::vector<Ref<MaterialPassStateEditor>> shaderPassState;
	};
}

