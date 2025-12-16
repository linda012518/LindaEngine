#include "RadioButton.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

RadioButton::RadioButton(bool p_selected, const std::string& p_label)
	: DataWidget<bool>(_selected), label(p_label)
{
	if (p_selected)
		Select();
}

void RadioButton::Select()
{
	_selected = true;
	ClickedEvent.Invoke(_radioID);
}

bool RadioButton::IsSelected() const
{
	return _selected;
}

bool RadioButton::GetRadioID() const
{
	return _radioID;
}

void RadioButton::_Draw_Impl()
{
	if (ImGui::RadioButton((label + _widgetID).c_str(), _selected))
	{
		ClickedEvent.Invoke(_radioID);
		this->NotifyChange();
	}
}
