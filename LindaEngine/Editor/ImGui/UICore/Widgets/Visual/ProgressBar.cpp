#include "ProgressBar.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::ProgressBar::ProgressBar(float p_fraction, const glm::vec2& p_size, const std::string& p_overlay) :
	fraction(p_fraction), size(p_size), overlay(p_overlay)
{
}

void LindaEditor::ProgressBar::_Draw_Impl()
{
	ImGui::ProgressBar(fraction, ImguiConverter::ToImVec2(size), !overlay.empty() ? overlay.c_str() : nullptr);
}
