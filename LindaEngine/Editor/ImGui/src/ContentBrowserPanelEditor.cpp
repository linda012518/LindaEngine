#include "ContentBrowserPanelEditor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(ContentBrowserPanelEditor, ImGuiPanelEditor)

void ContentBrowserPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Content Browser");

	ImGui::End();
}
