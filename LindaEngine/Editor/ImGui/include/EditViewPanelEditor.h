#pragma once

#include "AutoPtr.h"
#include "ImGuiPanelEditor.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace LindaEditor
{
	class EditViewPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(EditViewPanelEditor)
		EditViewPanelEditor();
		void OnImGuiRender();

		Ref<LindaEngine::RenderTexture> GetRenderTexture() { return _renderTexture; }

	private:
		Ref<LindaEngine::RenderTexture> _renderTexture;
		//glm::vec2 _viewportSize;
		LindaEngine::FramebufferTextureSpecification _fboSpec;
	};
}
