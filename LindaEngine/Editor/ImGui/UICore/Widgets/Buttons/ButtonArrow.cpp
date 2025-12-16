#include "ButtonArrow.h"

#include "imgui/imgui.h"

LindaEditor::ButtonArrow::ButtonArrow(Direction p_direction) : direction(p_direction)
{
}

void LindaEditor::ButtonArrow::_Draw_Impl()
{
	ImGuiDir dir;
	switch (direction)
	{
	case LindaEditor::Direction::NONE: dir = ImGuiDir_None; break;
	case LindaEditor::Direction::LEFT: dir = ImGuiDir_Left; break;
	case LindaEditor::Direction::RIGHT: dir = ImGuiDir_Right; break;
	case LindaEditor::Direction::UP: dir = ImGuiDir_Up; break;
	case LindaEditor::Direction::DOWN: dir = ImGuiDir_Down; break;
	}

	if (ImGui::ArrowButton(_widgetID.c_str(), dir))
	{
		ClickedEvent.Invoke();
	}
}
