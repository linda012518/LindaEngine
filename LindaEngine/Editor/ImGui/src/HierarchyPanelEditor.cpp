#include "HierarchyPanelEditor.h"
#include "SceneManagerEditor.h"
#include "Entity.h"
#include "Scene.h"
#include "NodeEditor.h"
#include "Transform.h"
#include "EventSystem.h"
#include "EventEditor.h"
#include "EventCodeEditor.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

static char renameBuffer[256] = "";

DYNAMIC_CREATE_CLASS(HierarchyPanelEditor, ImGuiPanelEditor)

void HierarchyPanelEditor::OnImGuiRender()
{
	bool close = true;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_UnsavedDocument;

	ImGui::Begin("Hierarchy", &close, window_flags);

	DrawEntitys();

	DrawBlankAreaDropTarget();

	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
	{
		_hoveredEntity = nullptr;
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			SelectNone();
		}
	}

	DrawContextMenu();

	ImGui::End();
}

void HierarchyPanelEditor::DrawEntitys()
{
	int index = 0;
	std::vector<Ref<Entity>>& entitys = SceneManagerEditor::GetCurrentNode()->scene->GetEntitys();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 0.0f));

	for (auto& go : entitys)
	{
		if (nullptr != go->GetTransform()->GetParent())
			continue;
		DrawEntityRecursive(go.get(), &index);
	}

	ImGui::PopStyleVar();
}

void HierarchyPanelEditor::DrawEntityRecursive(Entity* entity, int* index)
{
	entity->SetIndex(*index);
	*index += 1;

	const std::list<Transform*>& children = entity->GetTransform()->GetChildren();
	bool hasChildren = children.size() > 0;

	bool isSelected = IsEntitySelected(entity);
	ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (hasChildren)
	{
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	}
	else
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (isSelected)
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.50f, 0.80f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.20f, 0.50f, 0.80f, 1.00f));
	}

	bool opened = false;

	if (_renameEntity == entity)
	{
		// 重命名状态下：绘制一个空的树节点（用于布局）
		opened = ImGui::TreeNodeEx((void*)entity, flags, "");

		// 绘制输入框
		ImGui::SameLine();
		ImGui::SetKeyboardFocusHere();

		if (ImGui::InputText("##Rename", renameBuffer, IM_ARRAYSIZE(renameBuffer),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			// 按下回车确认重命名
			_renameEntity->SetName(renameBuffer);
			_renameEntity = nullptr;
		}

		// 失去焦点也确认重命名
		if (!ImGui::IsItemActive() && ImGui::IsItemDeactivated() && nullptr != _renameEntity)
		{
			_renameEntity->SetName(renameBuffer);
			_renameEntity = nullptr;
		}
		// ESC键取消重命名
		if (ImGui::IsKeyPressed(ImGuiKey_Escape) && nullptr != _renameEntity)
		{
			_renameEntity = nullptr;
		}
	}
	else
	{
		opened = ImGui::TreeNodeEx((void*)entity, flags, entity->GetName().c_str());
	}

	if (isSelected)
	{
		ImGui::PopStyleColor(2);
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		_mouseDownPos = ImGui::GetMousePos();
	}

	DragEntitys(entity);

	// 检查是否点击了展开三角区域
	bool clickedOnArrow = false;
	if (hasChildren && ImGui::IsItemHovered())
	{
		// 获取树节点的矩形区域
		ImVec2 rectMin = ImGui::GetItemRectMin();
		ImVec2 rectMax = ImGui::GetItemRectMax();
		// 获取鼠标位置
		ImVec2 mousePos = ImGui::GetMousePos();

		// 计算展开三角的近似区域（在ImGui中，箭头区域通常在左侧）
		float arrowWidth = ImGui::GetFrameHeight();
		ImRect arrowRect = ImRect(rectMin.x, rectMin.y, rectMin.x + arrowWidth, rectMax.y);

		// 检查鼠标是否在箭头区域内
		if (arrowRect.Contains(mousePos))
		{
			clickedOnArrow = true;
		}
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		ImVec2 mouseDelta = ImVec2(
			ImGui::GetMousePos().x - _mouseDownPos.x,
			ImGui::GetMousePos().y - _mouseDownPos.y
		);

		float dragThreshold = 5.0f;
		bool isDragging = std::abs(mouseDelta.x) > dragThreshold || std::abs(mouseDelta.y) > dragThreshold;

		if (false == isDragging && false == clickedOnArrow)
		{
			bool isCtrlDown = ImGui::GetIO().KeyCtrl;
			bool isShiftDown = ImGui::GetIO().KeyShift;
			HandleEntitySelection(entity, isCtrlDown, isShiftDown);
		}
	}

	if (ImGui::IsItemHovered() && _hoveredEntity != entity)
	{
		_hoveredEntity = entity;
	}

	if (opened)
	{
		if (hasChildren)
		{
			for (auto& child : children)
			{
				DrawEntityRecursive(&child->GetEntity(), index);
			}
		}
		ImGui::TreePop();
	}
}

void HierarchyPanelEditor::DrawContextMenu()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));       // 菜单栏背景
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));            // 文本颜色
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));         // 弹出菜单背景
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 选中状态（子菜单打开时）

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));   // 悬停背景
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));    // 激活背景
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 边框颜色
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));       // 分隔线颜色

	if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
	{
		if (nullptr != _hoveredEntity)
		{
			if (IsEntitySelected(_hoveredEntity) == false || _selectionEntityArray.size() <= 1)
			{
				SelectSingle();
				if (ImGui::MenuItem("Rename"))
				{
					_firstRename = true;
					_renameEntity = _selectionEntity;
					strcpy_s(renameBuffer, _renameEntity->GetName().c_str());
				}

				ImGui::Separator();
			}

			if (ImGui::MenuItem("Delete Entity"))
			{
				for (auto& go : _selectionEntityArray)
				{
					SceneManagerEditor::GetCurrentNode()->scene->DestroyEntityImmediately(go);
				}
				SelectNone();
			}

			ImGui::Separator();
		}

		if (ImGui::MenuItem("Create Empty Entity"))
		{
			SceneManagerEditor::GetCurrentNode()->scene->CreateEntity("Empty Entity");
		}
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(8);

}

void HierarchyPanelEditor::DrawBlankAreaDropTarget()
{
	if (false == _isDrag)
		return;

	// 添加一个覆盖整个窗口的透明拖放目标
	// 首先保存当前光标位置
	ImVec2 cursorPos = ImGui::GetCursorPos();

	// 将光标移到窗口左上角
	ImGui::SetCursorPos(ImVec2(0, 0));

	// 创建一个覆盖整个窗口的不可见按钮
	ImGui::InvisibleButton("##FullWindowDropTarget", ImGui::GetWindowSize());

	// 为这个按钮添加拖放目标
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
		{
			// 单个实体拖拽到空白区域
			Entity* draggedEntity = *(Entity**)payload->Data;
			if (draggedEntity && draggedEntity->GetTransform())
			{
				draggedEntity->GetTransform()->SetParent(nullptr);
			}
			_isDrag = false;
		}
		else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG_MULTI"))
		{
			// 多个实体拖拽到空白区域
			int count = payload->DataSize / sizeof(Entity*);
			Entity** draggedEntities = (Entity**)payload->Data;

			for (int i = 0; i < count; i++)
			{
				if (draggedEntities[i] && draggedEntities[i]->GetTransform())
				{
					draggedEntities[i]->GetTransform()->SetParent(nullptr);
				}
			}
			_isDrag = false;
		}

		ImGui::EndDragDropTarget();
	}

	// 恢复光标位置
	ImGui::SetCursorPos(cursorPos);
}

void HierarchyPanelEditor::DragEntitys(Entity* entity)
{
	if (ImGui::BeginDragDropSource())
	{
		_isDrag = true;
		if (_selectionEntityArray.size() > 1 && IsEntitySelected(entity))
		{
			std::vector<Entity*> draggingList(_selectionEntityArray.begin(), _selectionEntityArray.end());
			ImGui::SetDragDropPayload("ENTITY_DRAG_MULTI", draggingList.data(), draggingList.size() * sizeof(Entity*));
			ImGui::Text("Dragging");
		}
		else
		{
			SelectSingle();
			ImGui::SetDragDropPayload("ENTITY_DRAG", &_selectionEntity, sizeof(Entity*));
			ImGui::Text("Dragging");
		}
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
		{
			// 单个实体拖拽
			Entity* draggedEntity = *(Entity**)payload->Data;

			if (draggedEntity->GetTransform()->HasChild(entity->GetTransform()) == false && draggedEntity != entity)
			{
				Transform* parentTransform = entity->GetTransform();
				Transform* childTransform = draggedEntity->GetTransform();
				childTransform->SetParent(parentTransform);
			}
			_isDrag = false;
		}
		else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG_MULTI"))
		{
			int count = payload->DataSize / sizeof(Entity*);
			Entity** draggedEntities = (Entity**)payload->Data;

			bool isValid = true;
			std::vector<Entity*> allChildren;
			for (int i = 0; i < count; i++)
			{
				if (false == draggedEntities[i]->GetTransform()->HasChild(entity->GetTransform()))
					continue;
				isValid = false;
				break;
			}

			if (isValid)
			{
				Transform* parentTransform = entity->GetTransform();
				for (int i = 0; i < count; i++)
				{
					if (draggedEntities[i] != entity)
					{
						draggedEntities[i]->GetTransform()->SetParent(parentTransform);
					}
				}
			}
			_isDrag = false;
		}
		ImGui::EndDragDropTarget();
	}
}

void HierarchyPanelEditor::HandleEntitySelection(Entity* entity, bool isCtrlDown, bool isShiftDown)
{
	if (isShiftDown)
	{
		SelectRange(entity);
	}
	else if (isCtrlDown)
	{
		if (IsEntitySelected(entity))
		{
			DeselectEntity(entity);
			if (_selectionEntity == entity && !_selectionEntityArray.empty())
			{
				_selectionEntity = _selectionEntityArray.back();
			}
			else if (_selectionEntity == entity && _selectionEntityArray.empty())
			{
				_selectionEntity = nullptr;
			}
		}
		else
		{
			_selectionEntityArray.push_back(entity);
		}
	}
	else
	{
		SelectSingle();
	}
}

bool HierarchyPanelEditor::IsEntitySelected(Entity* entity)
{
	return std::find(_selectionEntityArray.begin(), _selectionEntityArray.end(), entity) != _selectionEntityArray.end();
}

void HierarchyPanelEditor::DeselectEntity(Entity* entity)
{
	auto itr = std::find(_selectionEntityArray.begin(), _selectionEntityArray.end(), entity);
	if (itr != _selectionEntityArray.end())
		_selectionEntityArray.erase(itr);
}

void HierarchyPanelEditor::SelectRange(Entity* entity)
{
	_selectionEntityArray.clear();
	if (nullptr == _selectionEntity)
	{
		SelectSingle();
	}
	else
	{
		int min = 0;
		int max = 0;
		if (_selectionEntity->GetIndex() > entity->GetIndex())
		{
			min = entity->GetIndex();
			max = _selectionEntity->GetIndex();
		}
		else
		{
			max = entity->GetIndex();
			min = _selectionEntity->GetIndex();
		}

		std::vector<Ref<Entity>>& entitys = SceneManagerEditor::GetCurrentNode()->scene->GetEntitys();
		for (auto& go : entitys)
		{
			int index = go->GetIndex();
			if (index < min || index > max)
				continue;
			_selectionEntityArray.push_back(go.get());
		}
	}
}

void HierarchyPanelEditor::SelectNone()
{
	_selectionEntity = nullptr;
	_selectionEntityArray.clear();

	SwitchSelectEntityEditor ssee;
	ssee.selectionEntity = nullptr;
	EventSystem::Dispatch(nullptr, EventCodeEditor::SwitchSelectEntity, ssee);
}

void HierarchyPanelEditor::SelectSingle()
{
	_selectionEntityArray.clear();
	_selectionEntity = _hoveredEntity;
	_selectionEntityArray.push_back(_selectionEntity);

	SwitchSelectEntityEditor ssee;
	ssee.selectionEntity = _selectionEntity;
	EventSystem::Dispatch(nullptr, EventCodeEditor::SwitchSelectEntity, ssee);
}

