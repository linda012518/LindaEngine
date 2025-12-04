#include "EditViewPanelEditor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(EditViewPanelEditor, ImGuiPanelEditor)

void EditViewPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Edit View");

	ImGui::End();

}
