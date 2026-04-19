#pragma once

#include "AutoPtr.h"
#include "ScriptablePass.h"

#include <vector>

namespace LindaEngine
{
	class PostProcessEffectRenderer;
	class RenderTexture;

	class PostProcessPass : public ScriptablePass
	{
	public:
		PostProcessPass();
		void Render(Weak<Camera> camera);

	private:
		void StackingCamera(Weak<Camera> camera, Ref<RenderTexture> outputRT);

	private:
		Ref<RenderTexture> _source;
		Ref<RenderTexture> _dest;

		static Ref<RenderTexture> _multipleCameraRT;
	};
}

