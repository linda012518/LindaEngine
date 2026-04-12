#pragma once

#include "AutoPtr.h"
#include "ISerializable.h"
#include "Texture.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class PostProcessEffectRenderer : public ISerializable
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Render(Ref<RenderTexture> src, Ref<RenderTexture> dest) = 0;

		void SetEnable(bool enable) { _enable = enable; }
		bool IsEnable() { return _enable; }

		virtual std::string GetClassName() { return "PostProcessEffectRenderer"; }

		static std::vector<std::string>& GetPostProcessNames();
		static int CollectPostProcess(std::string name);

	protected:
		bool _enable;

	};

#define COLLECT_POSTPROCESS(CLASS_NAME) \
	static int g_tmpPostProcess_##CLASS_NAME = PostProcessEffectRenderer::CollectPostProcess(#CLASS_NAME);
}
