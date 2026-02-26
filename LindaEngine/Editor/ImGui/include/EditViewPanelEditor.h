#pragma once

#include "AutoPtr.h"
#include "IEventHandler.h"
#include "ImGuiPanelEditor.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace LindaEngine
{
	class Entity;
}

namespace LindaEditor
{
	class EditViewPanelEditor : public ImGuiPanelEditor, public LindaEngine::IEventHandler
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(EditViewPanelEditor)
		EditViewPanelEditor();
		void OnImGuiRender();

		void OnEvent(LindaEngine::IEventHandler* sender, int eventCode, LindaEngine::Event& eventData);

		Ref<LindaEngine::RenderTexture> GetRenderTexture() { return _renderTexture; }

	private:
		void ProcessPick(ImVec2& viewportPanelSize, ImVec2& windowPos);
		void DrawRect(ImVec2& windowPos);
		void RenderGuizmo();

	private:
		Ref<LindaEngine::RenderTexture> _renderTexture;
		std::vector<LindaEngine::FramebufferTextureSpecification> _fboSpecs;
		LindaEngine::FramebufferTextureSpecification _pickSpec;
		bool _isLeftPressed = false;
		bool _isLeftFirst = true;
		glm::ivec2 _firstPos;

		LindaEngine::Entity* _selectionEntity = nullptr;

	public:
		static bool hovered;
		static Ref<LindaEngine::RenderTexture> pickRT;
	};
}
