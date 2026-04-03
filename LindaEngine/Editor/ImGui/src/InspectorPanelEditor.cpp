#include "InspectorPanelEditor.h"
#include "EventSystemEditor.h"
#include "Entity.h"
#include "EventEditor.h"
#include "EventCodeEditor.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "GUILayoutEditor.h"
#include "Material.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(InspectorPanelEditor, ImGuiPanelEditor)

InspectorPanelEditor::InspectorPanelEditor()
{
	EventSystemEditor::Bind(EventCodeEditor::SwitchSelectEntity, this);
	EventSystemEditor::Bind(EventCodeEditor::SwitchInspectorObject, this);
}

void InspectorPanelEditor::OnImGuiRender()
{
	bool close = true;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Inspector", &close, window_flags);

	if (nullptr == _selectionEntity)
	{
		_showPopup = false;
		ImGui::End();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	bool ret = _selectionEntity->IsActiveSelf();
	ImGui::Checkbox(std::string("##" + _selectionEntity->GetName()).c_str(), &ret);
	if (_selectionEntity->IsActiveSelf() != ret)
		_selectionEntity->SetActive(ret);

	ImGui::SameLine();
	std::string go = _selectionEntity->GetName();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18.0f);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##Name", &go);
	if (go != _selectionEntity->GetName())
		_selectionEntity->SetName(go);

	if (nullptr != _object)
		_object->OnImguiRender();
	DrawComponents();
	DrawSundry();

	ImGui::End();
}

void InspectorPanelEditor::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	switch (eventCode)
	{
	case EventCodeEditor::SwitchSelectEntity:
	{
		SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
		_selectionEntity = event.selectionEntity;
	}
	break;
	case EventCodeEditor::SwitchInspectorObject:
	{
		SwitchInspectorObjectEditor& event = dynamic_cast<SwitchInspectorObjectEditor&>(eventData);
		_object = event.lobject;
	}
	break;
	}
}

void InspectorPanelEditor::DrawComponents()
{
	if (nullptr == _selectionEntity)
		return;

	_selectionEntity->OnImguiRender();
}

void InspectorPanelEditor::DrawSundry()
{
	std::vector<std::string>& names = ComponentFactory::GetComponents();

	GUILayoutEditor::Dropdown("Add Component", names, [&](int index) {
		_selectionEntity->AddComponent(names[index]);
		});
}

