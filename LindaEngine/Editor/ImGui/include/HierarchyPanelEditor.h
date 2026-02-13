#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"

#include <vector>

namespace LindaEngine
{
	class Entity;
}

namespace LindaEditor
{
	class HierarchyPanelEditor : public ImGuiPanelEditor, public LindaEngine::IEventHandler
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(HierarchyPanelEditor)
		HierarchyPanelEditor();
		void OnImGuiRender();

		void OnEvent(LindaEngine::IEventHandler* sender, int eventCode, LindaEngine::Event& eventData);

	private:
		void DrawEntitys();
		void DrawEntityRecursive(LindaEngine::Entity* entity, int* index);
		void DrawContextMenu();

		void DrawBlankAreaDropTarget();
		void DragEntitys(LindaEngine::Entity* entity);
		void HandleEntitySelection(LindaEngine::Entity* entity, bool isCtrlDown, bool isShiftDown);
		bool IsEntitySelected(LindaEngine::Entity* entity);
		void DeselectEntity(LindaEngine::Entity* entity);
		void SelectRange(LindaEngine::Entity* entity);

		void SelectNone();
		void SelectSingle();
		void SendSwitchEntityMessage();

	private:
		LindaEngine::Entity* _selectionEntity = nullptr;
		LindaEngine::Entity* _hoveredEntity = nullptr;
		LindaEngine::Entity* _renameEntity = nullptr;
		bool _firstRename = false;
		std::vector<LindaEngine::Entity*> _selectionEntityArray;
		ImVec2 _mouseDownPos;
	};
}
