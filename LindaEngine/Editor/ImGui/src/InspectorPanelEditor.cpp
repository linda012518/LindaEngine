#include "InspectorPanelEditor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(InspectorPanelEditor, ImGuiPanelEditor)

void InspectorPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Inspector");

	ImGui::End();
}
