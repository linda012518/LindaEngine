#include "CheckBox.h"

#include "imgui/imgui.h"

LindaEditor::CheckBox::CheckBox(bool p_value, const std::string& p_label) :
	DataWidget<bool>(value), value(p_value), label(p_label)
{
}

void LindaEditor::CheckBox::_Draw_Impl()
{
	bool previousValue = value;

	ImGui::Checkbox((label + _widgetID).c_str(), &value);

	if (value != previousValue)
	{
		ValueChangedEvent.Invoke(value);
		this->NotifyChange();
	}
}
