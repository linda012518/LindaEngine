#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEngine
{
	class PostProcessEffectRenderer;
}

namespace LindaEditor
{
	class EnvironmentPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(EnvironmentPanelEditor)
		void OnImGuiRender();

	private:
		void DrawSkyboxMaterial();
		void DrawPostProcessEffect();
		void DrawLayerMaskList();

	private:
		Ref<LindaEngine::PostProcessEffectRenderer> _postProcessDirty = nullptr;
	};
}
