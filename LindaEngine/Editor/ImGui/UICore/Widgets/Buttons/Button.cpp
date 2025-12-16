#include "Button.h"
#include "ImguiConverter.h"
#include "imgui/imgui.h"

LindaEditor::Button::Button(const std::string& p_label, const glm::vec2& p_size, bool p_disabled)
	: label(p_label), size(p_size), disabled(p_disabled)
{
	auto& style = ImGui::GetStyle();

	idleBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_Button]);
	hoveredBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_ButtonHovered]);
	clickedBackgroundColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_ButtonActive]);
	textColor = ImguiConverter::ToGLMVec4(style.Colors[ImGuiCol_Text]);
}

void LindaEditor::Button::_Draw_Impl()
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

	// Instead of using disabled directly, as its value can change if some
	// callback is bound to the ClickedEvent.
	const bool isDisabled = disabled;

	if (isDisabled)
	{
		ImGui::BeginDisabled();
	}

	if (ImGui::Button((label + _widgetID).c_str(), ImVec2(size.x, size.y)))
	{
		ClickedEvent.Invoke();
	}

	if (isDisabled)
	{
		ImGui::EndDisabled();
	}

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;

}
