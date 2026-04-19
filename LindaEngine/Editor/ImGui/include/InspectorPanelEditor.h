#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"

namespace LindaEngine
{
	class LObject;
}

namespace LindaEditor
{
	class InspectorPanelEditor : public std::enable_shared_from_this<InspectorPanelEditor>, public ImGuiPanelEditor, public LindaEngine::IEventHandler
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(InspectorPanelEditor)
		InspectorPanelEditor();
		void OnImGuiRender();

		void OnEvent(Weak<LindaEngine::IEventHandler> sender, int eventCode, LindaEngine::Event& eventData);

	private:
		void Initialize();
		void DrawSundry();

	private:
		Weak<LindaEngine::LObject> _object = nullptr;
	};
}
