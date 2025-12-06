#include "HierarchyPanelEditor.h"
#include "SceneManagerEditor.h"
#include "Entity.h"
#include "Scene.h"
#include "NodeEditor.h"
#include "Transform.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(HierarchyPanelEditor, ImGuiPanelEditor)

void HierarchyPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Hierarchy");

	DrawEntitys();

	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
	{
		_hoveredEntity = nullptr;
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			_selectionEntity = nullptr;
			_selectionEntityArray.clear();
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

	bool opened = ImGui::TreeNodeEx((void*)entity, flags, entity->GetName().c_str());

	if (isSelected)
	{
		ImGui::PopStyleColor(2);
	}

	DragEntitys(entity);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && _isDrag == false)
	{
		bool isCtrlDown = ImGui::GetIO().KeyCtrl;
		bool isShiftDown = ImGui::GetIO().KeyShift;
		HandleEntitySelection(entity, isCtrlDown, isShiftDown);
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
			_selectionEntityArray.clear();
			_selectionEntity = _hoveredEntity;
			_selectionEntityArray.push_back(_selectionEntity);
			if (ImGui::MenuItem("Rename"))
			{

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Delete Entity"))
			{
				SceneManagerEditor::GetCurrentNode()->scene->DestroyEntityImmediately(_selectionEntity);
				_selectionEntity = nullptr;
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
			_selectionEntity = entity;
			_selectionEntityArray.clear();
			_selectionEntityArray.push_back(entity);
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
		}
		_isDrag = false;
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
		_selectionEntityArray.clear();
		_selectionEntityArray.push_back(entity);
		_selectionEntity = entity;
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
		_selectionEntity = entity;
		_selectionEntityArray.push_back(entity);
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

