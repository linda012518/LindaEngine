#include "ColorEdit.h"

#include "imgui/imgui.h"

LindaEditor::ColorEdit::ColorEdit(bool p_enableAlpha, const glm::vec4& p_defaultColor) :
	DataWidget<glm::vec4>(color), enableAlpha(p_enableAlpha), color(p_defaultColor)
{
}

void LindaEditor::ColorEdit::_Draw_Impl()
{
	int flags = !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (enableAlpha)
		valueChanged = ImGui::ColorEdit4(_widgetID.c_str(), &color.r, flags);
	else
		valueChanged = ImGui::ColorEdit3(_widgetID.c_str(), &color.r, flags);

	if (valueChanged)
	{
		ColorChangedEvent.Invoke(color);
		this->NotifyChange();
	}
}
