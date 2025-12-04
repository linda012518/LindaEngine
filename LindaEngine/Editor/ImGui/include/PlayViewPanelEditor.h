#pragma once

#include "AutoPtr.h"
#include "ImGuiPanelEditor.h"
#include "Texture.h"

namespace LindaEditor
{
	class PlayViewPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(PlayViewPanelEditor)
		PlayViewPanelEditor();
		void OnImGuiRender();

		Ref<LindaEngine::RenderTexture> GetRenderTexture() { return _renderTexture; }

	private:
		Ref<LindaEngine::RenderTexture> _renderTexture;
		LindaEngine::FramebufferTextureSpecification _fboSpec;
	};
}