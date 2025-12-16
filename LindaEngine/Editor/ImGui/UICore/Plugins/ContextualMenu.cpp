#include "ContextualMenu.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

void ContextualMenu::Execute(PluginExecutionContext p_context)
{
	if (p_context == PluginExecutionContext::PANEL ? ImGui::BeginPopupContextWindow() : ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void ContextualMenu::ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
