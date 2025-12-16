#include "PanelMenuBar.h"

#include "imgui/imgui.h"

void LindaEditor::PanelMenuBar::_Draw_Impl()
{
	if (!_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}
