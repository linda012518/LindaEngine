#include "MaterialPass.h"
#include "Global.hpp"
#include "glad/glad.h"

using namespace LindaEngine;

void MaterialPass::Setup()
{
	RenderState& state = Global::renderState;
	if (state.colorMask != _renderState.colorMask)
	{
		state.colorMask = _renderState.colorMask;
		glColorMask(state.colorMask.R, state.colorMask.G, state.colorMask.B, state.colorMask.A);
	}

	if (state.depthState.depthTest != _renderState.depthState.depthTest)
	{

	}
}

void MaterialPass::AddKeyword(std::string& key)
{
	auto itr = std::find(_keywords.begin(), _keywords.end(), key);
	if (itr == _keywords.end())
		_keywords.push_back(key);
}
