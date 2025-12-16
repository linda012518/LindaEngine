#include "PlotHistogram.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::PlotHistogram::PlotHistogram(const std::vector<float>& p_data, float p_minScale, float p_maxScale, const glm::vec2& p_size, const std::string& p_overlay, const std::string& p_label, int p_forceHover)
	: Plot(p_data, p_minScale, p_maxScale, p_size, p_overlay, p_label, p_forceHover)
{
}

void LindaEditor::PlotHistogram::_Draw_Impl()
{
	// TODO: Use forceHover
	ImGui::PlotHistogram((label + _widgetID).c_str(), data.data(), static_cast<int>(data.size()), 0, overlay.c_str(), minScale, maxScale, ImguiConverter::ToImVec2(size), sizeof(float));
}
