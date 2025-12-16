#include "Text.h"

#include "imgui/imgui.h"

LindaEditor::Text::Text(const std::string& p_content) :
	DataWidget(content),
	content(p_content)
{
}

void LindaEditor::Text::_Draw_Impl()
{
	ImGui::Text(content.c_str());
}
