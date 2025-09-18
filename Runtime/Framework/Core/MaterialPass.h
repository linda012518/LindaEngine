#pragma once

#include "LObject.hpp"
#include "RenderState.h"
#include "ShaderUniform.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Shader;

	enum class PassMode
	{
		Common, Depth, ShadowCaster, DepthNormal
	};
	enum class RenderType
	{
		Opaque, Skybox, Transparent, Overlay
	};

	class MaterialPass : public LObject
	{
	public:
		MaterialPass();
		virtual ~MaterialPass();

		void Setup();

		void AddKeyword(std::string& key, bool dynamic = false);

		void UpdateUniforms();

	private:
		void CheckColorMask(RenderState& state);
		void CheckDepthState(RenderState& state);
		void CheckCullFaceState(RenderState& state);
		void CheckPolygonModeState(RenderState& state);
		void CheckStencilState(RenderState& state);
		void CheckBlendState(RenderState& state);

	private:
		std::unordered_map<std::string, ShaderUniform> _uniformMap;
		std::vector<std::string> _keywords; //关键字宏定义
		std::vector<std::string> _keywordsDynamic; //动态添加的关键字宏定义
		PassMode _passMode = PassMode::Common;
		RenderType _renderType = RenderType::Opaque; //是否不透明物体
		int _renderQueue = 2000; //渲染队列
		bool _ShadowCast = true; //是否投射阴影
		bool _receiveShadow = true; //是否接收阴影

		RenderState _renderState;
	};
}
