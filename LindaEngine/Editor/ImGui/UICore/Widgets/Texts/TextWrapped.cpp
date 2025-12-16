#include "TextWrapped.h"

#include "imgui/imgui.h"

LindaEditor::TextWrapped::TextWrapped(const std::string& p_content) :
	Text(p_content)
{
}

void LindaEditor::TextWrapped::_Draw_Impl()
{
	ImGui::TextWrapped(content.c_str());
}
