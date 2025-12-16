#include "TextClickable.h"

#include "imgui/imgui.h"

LindaEditor::TextClickable::TextClickable(const std::string& p_content) :
	Text(p_content)
{
}

void LindaEditor::TextClickable::_Draw_Impl()
{
	bool useless = false;

	if (ImGui::Selectable((content + _widgetID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			DoubleClickedEvent.Invoke();
		}
		else
		{
			ClickedEvent.Invoke();
		}
	}
}
