#include "TextColored.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::TextColored::TextColored(const std::string& p_content, const glm::vec4& p_color) :
	Text(p_content), color(p_color)
{
}

void LindaEditor::TextColored::_Draw_Impl()
{
	ImGui::TextColored(ImguiConverter::ToImVec4(color), content.c_str());
}
