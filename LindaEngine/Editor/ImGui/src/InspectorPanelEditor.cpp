#include "InspectorPanelEditor.h"
#include "EventSystemEditor.h"
#include "Entity.h"
#include "EventEditor.h"
#include "EventCodeEditor.h"
#include "glm/glm.hpp"
#include "ComponentFactory.h"
#include "GUILayoutEditor.h"
#include "Texture.h"

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

	if (nullptr == _object)
	{
		ImGui::End();
		return;
	}

	if (nullptr != _object)
	{
		Texture* texture = dynamic_cast<Texture*>(_object);
		if (nullptr == texture)
			_object->OnImguiRender();
		else
			Texture::OnImguiRender(texture);
	}

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
		_object = event.selectionEntity;
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

void InspectorPanelEditor::DrawSundry()
{
	if (nullptr == _object)
		return;

	Entity* entity = dynamic_cast<Entity*>(_object);
	if (nullptr == entity)
		return;

	std::vector<std::string>& names = ComponentFactory::GetComponents();

	GUILayoutEditor::Dropdown("Add Component", names, [&](int index) {
		entity->AddComponent(names[index]);
		});
}

