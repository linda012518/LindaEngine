#include "TextLabelled.h"

#include "imgui/imgui.h"

LindaEditor::TextLabelled::TextLabelled(const std::string& p_content, const std::string& p_label) :
	Text(p_content), label(p_label)
{
}

void LindaEditor::TextLabelled::_Draw_Impl()
{
	ImGui::LabelText((label + _widgetID).c_str(), content.c_str());
}
