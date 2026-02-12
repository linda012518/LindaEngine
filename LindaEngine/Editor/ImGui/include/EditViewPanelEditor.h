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
		std::vector<LindaEngine::FramebufferTextureSpecification> _fboSpecs;
		LindaEngine::FramebufferTextureSpecification _pickSpec;
		bool _isLeftPressed = false;
		bool _isLeftFirst = true;
		ImVec2 _firstPos;

	public:
		static bool hovered;
		static Ref<LindaEngine::RenderTexture> pickRT;
	};
}
