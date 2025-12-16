#include "ButtonColored.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::ButtonColored::ButtonColored(const std::string& p_label, const glm::vec4& p_color, const glm::vec2& p_size, bool p_enableAlpha)
	: label(p_label), color(p_color), size(p_size), enableAlpha(p_enableAlpha)
{
}

void LindaEditor::ButtonColored::_Draw_Impl()
{
	ImVec4 imColor = ImguiConverter::ToImVec4(color);

	if (ImGui::ColorButton((label + _widgetID).c_str(), imColor, !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0, ImguiConverter::ToImVec2(size)))
		ClickedEvent.Invoke();

	color = ImguiConverter::ToGLMVec4(imColor);
}
