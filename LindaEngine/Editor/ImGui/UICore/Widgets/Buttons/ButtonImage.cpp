#include "ButtonImage.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

LindaEditor::ButtonImage::ButtonImage(uint32_t p_textureID, const glm::vec2& p_size) : textureID{ p_textureID }, size(p_size)
{
}

void LindaEditor::ButtonImage::_Draw_Impl()
{
	ImVec4 bg = ImguiConverter::ToImVec4(background);
	ImVec4 tn = ImguiConverter::ToImVec4(tint);

	// Instead of using disabled directly, as its value can change if some
	// callback is bound to the ClickedEvent.
	const bool isDisabled = disabled;

	if (isDisabled)
	{
		ImGui::BeginDisabled();
	}

	if (ImGui::ImageButtonEx(ImGui::GetID(_widgetID.c_str()), textureID.id, ImguiConverter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), bg, tn))
	{
		ClickedEvent.Invoke();
	}

	if (isDisabled)
	{
		ImGui::EndDisabled();
	}

}
