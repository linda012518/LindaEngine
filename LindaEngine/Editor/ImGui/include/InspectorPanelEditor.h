#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"

namespace LindaEngine
{
	class LObject;
}

namespace LindaEditor
{
	class InspectorPanelEditor : public ImGuiPanelEditor, public LindaEngine::IEventHandler
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(InspectorPanelEditor)
		InspectorPanelEditor();
		void OnImGuiRender();

		void OnEvent(LindaEngine::IEventHandler* sender, int eventCode, LindaEngine::Event& eventData);

	private:
		void DrawSundry();

	private:
		LindaEngine::LObject* _object = nullptr;
	};
}
