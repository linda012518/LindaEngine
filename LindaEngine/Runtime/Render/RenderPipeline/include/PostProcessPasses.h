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
		void Render(Camera* camera);

	private:
		Ref<RenderTexture> _source;
		Ref<RenderTexture> _dest;
	};
}

