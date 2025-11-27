#pragma once

#include "AutoPtr.h"
#include "ISerializable.h"
#include "Texture.h"

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

	protected:
		bool _enable;
	};
}
