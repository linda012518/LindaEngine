#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace LindaEngine { struct ShaderUniform; struct RenderState; }

namespace LindaEditor
{
	struct MaterialPassState
	{
		std::unordered_map<std::string, Ref<LindaEngine::ShaderUniform>> _uniformNameMap;
		std::vector<std::string> _keywords;
		std::string _lightMode;
		Ref<LindaEngine::RenderState> renderState;
	};

	struct MaterialState
	{
		bool hasFallback;
		int renderQueue;
		LindaEngine::RenderType renderType;
		std::string shaderPath;
		std::string materialPath;
		std::vector<Ref<MaterialPassState>> shaderSrcCode;
	};

	class ShaderLoaderEditor
	{
	public:
		static Ref<MaterialState> Load(const char* path);
	};
}

