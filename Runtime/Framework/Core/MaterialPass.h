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
		std::vector<std::string> _keywords; //�ؼ��ֺ궨��
		PassMode _passMode = PassMode::Common;
		bool _isOpaque = true; //�Ƿ�͸������
		int _renderQueue = 2000; //��Ⱦ����
		bool _ShadowCast = true; //�Ƿ�Ͷ����Ӱ
		bool _receiveShadow = true; //�Ƿ������Ӱ

		RenderState _renderState;
	};
}
