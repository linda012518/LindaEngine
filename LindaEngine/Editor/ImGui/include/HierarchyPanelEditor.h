#pragma once

#include "ImGuiPanelEditor.h"
#include "IEventHandler.h"

#include <vector>

namespace LindaEngine
{
	class Entity;
	class CameraController;
	class Transform;
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
		static std::vector<LindaEngine::Entity*>& GetSelectedEntity() { return _selectionEntityArray; }

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

		void SetEntityPosition(LindaEngine::Entity* entity, LindaEngine::Transform* parent);

	private:
		LindaEngine::Entity* _selectionEntity = nullptr;
		LindaEngine::Entity* _hoveredEntity = nullptr;
		LindaEngine::Entity* _renameEntity = nullptr;
		LindaEngine::CameraController* _cameraCtrl = nullptr;
		bool _firstRename = false;
		ImVec2 _mouseDownPos;
		static std::vector<LindaEngine::Entity*> _selectionEntityArray;
	};
}
