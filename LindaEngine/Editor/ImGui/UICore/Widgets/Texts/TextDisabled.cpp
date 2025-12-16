#include "TextDisabled.h"

#include "imgui/imgui.h"

LindaEditor::TextDisabled::TextDisabled(const std::string& p_content) :
	Text(p_content)
{
}

void LindaEditor::TextDisabled::_Draw_Impl()
{
	ImGui::TextDisabled(content.c_str());
}
