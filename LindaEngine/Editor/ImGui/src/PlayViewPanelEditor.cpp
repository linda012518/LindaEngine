#include "PlayViewPanelEditor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(PlayViewPanelEditor, ImGuiPanelEditor)

void PlayViewPanelEditor::OnImGuiRender()
{
	ImGui::Begin("Play View");

	ImGui::End();

}
