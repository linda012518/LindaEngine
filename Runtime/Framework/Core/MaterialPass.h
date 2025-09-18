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
		std::vector<std::string> _keywords; //�ؼ��ֺ궨��
		std::vector<std::string> _keywordsDynamic; //��̬��ӵĹؼ��ֺ궨��
		PassMode _passMode = PassMode::Common;
		RenderType _renderType = RenderType::Opaque; //�Ƿ�͸������
		int _renderQueue = 2000; //��Ⱦ����
		bool _ShadowCast = true; //�Ƿ�Ͷ����Ӱ
		bool _receiveShadow = true; //�Ƿ������Ӱ

		RenderState _renderState;
	};
}
