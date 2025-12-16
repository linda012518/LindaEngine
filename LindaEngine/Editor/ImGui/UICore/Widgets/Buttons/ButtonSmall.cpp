#include "ButtonSmall.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::ButtonSmall::ButtonSmall(const std::string& p_label) : label(p_label)
{
	auto& style = ImGui::GetStyle();

	idleBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_Button]);
	hoveredBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_ButtonHovered]);
	clickedBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_ButtonActive]);
	textColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_Text]);
}

void LindaEditor::ButtonSmall::_Draw_Impl()
{
	auto& style = ImGui::GetStyle();

	auto defaultIdleColor = style.Colors[ImGuiCol_Button];
	auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	auto defaultTextColor = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button] = ImguiConverter::ToImVec4(idleBackgroundColor);
	style.Colors[ImGuiCol_ButtonHovered] = ImguiConverter::ToImVec4(hoveredBackgroundColor);
	style.Colors[ImGuiCol_ButtonActive] = ImguiConverter::ToImVec4(clickedBackgroundColor);
	style.Colors[ImGuiCol_Text] = ImguiConverter::ToImVec4(textColor);

	if (ImGui::SmallButton((label + _widgetID).c_str()))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;

}
