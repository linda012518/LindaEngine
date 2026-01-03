#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"
#include "EventEditor.h"

namespace LindaEngine
{
	class Entity;
	class Component;
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
		void DrawComponents();

	private:
		LindaEngine::Entity* _selectionEntity = nullptr;
		LindaEngine::Component* _component = nullptr;
	};
}
