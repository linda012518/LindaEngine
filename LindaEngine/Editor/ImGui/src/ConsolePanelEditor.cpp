#include "ConsolePanelEditor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(ConsolePanelEditor, ImGuiPanelEditor)

void ConsolePanelEditor::OnImGuiRender()
{
	ImGui::Begin("Console");
	for (size_t i = 0; i < 20; i++)
	{
		ImGui::Text("Hello, world!");

	}
	ImGui::End();

}