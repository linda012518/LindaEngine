#include "MenuList.h"

#include "imgui/imgui.h"

LindaEditor::MenuList::MenuList(const std::string& p_name, bool p_locked) :
	name(p_name), locked(p_locked)
{
}

void LindaEditor::MenuList::_Draw_Impl()
{
	if (ImGui::BeginMenu(name.c_str(), !locked))
	{
		if (!_opened)
		{
			ClickedEvent.Invoke();
			_opened = true;
		}

		DrawWidgets();
		ImGui::EndMenu();
	}
	else
	{
		_opened = false;
	}
}
