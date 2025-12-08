#pragma once

#include "ImGuiPanelEditor.h"

#include <vector>

namespace LindaEngine
{
	class Entity;
}

namespace LindaEditor
{
	class HierarchyPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(HierarchyPanelEditor)
		void OnImGuiRender();

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

	private:
		LindaEngine::Entity* _selectionEntity;
		LindaEngine::Entity* _hoveredEntity;
		LindaEngine::Entity* _renameEntity;
		bool _firstRename = false;
		std::vector<LindaEngine::Entity*> _selectionEntityArray;
		ImVec2 _mouseDownPos;
		bool _isDrag = false;
	};
}
