#pragma once

#include "LObject.hpp"
#include "RenderState.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class Shader;

	enum PassMode
	{
		Common, Depth, Shadow, DepthNormal
	};

	class MaterialPass : public LObject
	{
	public:
		MaterialPass();
		virtual ~MaterialPass();

		void Setup();

		void AddKeyword(std::string& key);

	private:


	private:
		std::vector<std::string> _keywords; //关键字宏定义
		PassMode _passMode = PassMode::Common;
		bool _isOpaque = true; //是否不透明物体
		int _renderQueue = 2000; //渲染队列
		bool _ShadowCast = true; //是否投射阴影
		bool _receiveShadow = true; //是否接收阴影

		RenderState _renderState;
	};
}
