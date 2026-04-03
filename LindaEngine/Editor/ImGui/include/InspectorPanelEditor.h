#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"

namespace LindaEngine
{
	class Entity;
	class Component;
	class Material;
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
		void DrawComponents();
		void DrawSundry();

	private:
		LindaEngine::LObject* _object = nullptr;
		LindaEngine::Entity* _selectionEntity = nullptr;
		bool _showPopup = false;
	};
}
