#include "PostProcessEffectRenderer.h"

using namespace LindaEngine;

std::vector<std::string>& PostProcessEffectRenderer::GetPostProcessNames()
{
	static std::vector<std::string> components;
	return components;
}

int PostProcessEffectRenderer::CollectPostProcess(std::string name)
{
	GetPostProcessNames().push_back(name);
	return 0;
}