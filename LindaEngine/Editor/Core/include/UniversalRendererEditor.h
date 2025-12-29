#pragma once

#include "ScriptableRenderer.h"

namespace LindaEditor
{
	class UniversalRendererEditor : public LindaEngine::ScriptableRenderer
	{
	protected:
		void CollectRenderPass();
	};
}
