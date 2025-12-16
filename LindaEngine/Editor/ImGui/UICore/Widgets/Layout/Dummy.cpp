#include "Dummy.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::Dummy::Dummy(const glm::vec2& p_size) : size(p_size)
{
}

void LindaEditor::Dummy::_Draw_Impl()
{
	ImGui::Dummy(ImguiConverter::ToImVec2(size));
}
