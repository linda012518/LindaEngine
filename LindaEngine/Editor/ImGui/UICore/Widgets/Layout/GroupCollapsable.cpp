#include "GroupCollapsable.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

LindaEditor::GroupCollapsable::GroupCollapsable(const std::string& p_name) : name(p_name)
{
}

void LindaEditor::GroupCollapsable::_Draw_Impl()
{
	bool previouslyOpened = opened;

	ImGui::SetNextItemOpen(!collapsed);

	if (ImGui::CollapsingHeader(name.c_str(), closable ? &opened : nullptr))
		Group::_Draw_Impl();

	collapsed = !ImGui::TreeNodeUpdateNextOpen(ImGui::GetID(name.c_str()), ImGuiTreeNodeFlags_None);

	if (opened != previouslyOpened)
	{
		if (opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}
