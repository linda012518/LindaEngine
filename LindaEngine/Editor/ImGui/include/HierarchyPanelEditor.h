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
	class HierarchyPanelEditor : public std::enable_shared_from_this<HierarchyPanelEditor>, public ImGuiPanelEditor, public LindaEngine::IEventHandler
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(HierarchyPanelEditor)
		HierarchyPanelEditor();
		void OnImGuiRender();

		void OnEvent(Weak<LindaEngine::IEventHandler> sender, int eventCode, LindaEngine::Event& eventData);
		static std::vector<Weak<LindaEngine::Entity>>& GetSelectedEntity() { return _selectionEntityArray; }

	private:
		void Initialize();
		void DrawEntitys();
		void DrawEntityRecursive(Weak<LindaEngine::Entity> entity, int* index);
		void DrawContextMenu();

		void DrawBlankAreaDropTarget();
		void DragEntitys(Weak<LindaEngine::Entity> entity);
		void HandleEntitySelection(Weak<LindaEngine::Entity> entity, bool isCtrlDown, bool isShiftDown);
		bool IsEntitySelected(Weak<LindaEngine::Entity> entity);
		void DeselectEntity(Weak<LindaEngine::Entity> entity);
		void SelectRange(Weak<LindaEngine::Entity> entity);

		void SelectNone();
		void SelectSingle();
		void SendSwitchEntityMessage();

		void SetEntityPosition(Weak<LindaEngine::Entity> entity, Weak<LindaEngine::Transform> parent);

	private:
		Weak<LindaEngine::Entity> _selectionEntity = nullptr;
		Weak<LindaEngine::Entity> _hoveredEntity = nullptr;
		Weak<LindaEngine::Entity> _renameEntity = nullptr;
		Weak<LindaEngine::CameraController> _cameraCtrl = nullptr;
		bool _firstRename = false;
		ImVec2 _mouseDownPos;
		static std::vector<Weak<LindaEngine::Entity>> _selectionEntityArray;
	};
}
