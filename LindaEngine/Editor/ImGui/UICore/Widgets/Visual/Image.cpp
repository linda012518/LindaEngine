#include "Image.h"
#include "ImguiConverter.h"

#include "imgui/imgui.h"

LindaEditor::Image::Image(uint32_t p_textureID, const glm::vec2& p_size) :
	textureID{ p_textureID }, size(p_size)
{
}

void LindaEditor::Image::_Draw_Impl()
{
	ImGui::Image(textureID.id, ImguiConverter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
}
