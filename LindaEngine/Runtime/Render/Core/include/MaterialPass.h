#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "RenderState.h"
#include "ShaderUniform.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Shader;

	class MaterialPass : public LObject
	{
		friend class YamlSerializer;
		friend class Material;

	public:
		void Setup();

		void AddKeyword(std::string& key);

		void UpdateUniforms();

	private:
		void CheckColorMask(RenderState& state);
		void CheckDepthState(RenderState& state);
		void CheckCullFaceState(RenderState& state);
		void CheckPolygonModeState(RenderState& state);
		void CheckStencilState(RenderState& state);
		void CheckBlendState(RenderState& state);

	private:
		std::unordered_map<std::string, Ref<ShaderUniform>> _uniformNameMap; //从shader文件解析
		std::vector<std::string> _keywords; //关键字宏定义，全局的从其它配置添加，不记录
		std::string _lightMode = "Color";
		int _renderQueue = 2000; //渲染队列

		RenderState _renderState; //材质需要的状态
		static Ref<MaterialPass> overrideMatPass;

	private:
		static RenderState _currentState; //当前渲染正在用的状态
		static RenderState _defualtState;
	};
}
