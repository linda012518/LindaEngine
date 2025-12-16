#include "ComboBox.h"

#include "imgui/imgui.h"

LindaEditor::ComboBox::ComboBox(int p_currentChoice) :
	DataWidget<int>(currentChoice), currentChoice(p_currentChoice)
{
}

void LindaEditor::ComboBox::_Draw_Impl()
{
	if (choices.find(currentChoice) == choices.end())
		currentChoice = choices.begin()->first;

	if (ImGui::BeginCombo(_widgetID.c_str(), choices[currentChoice].c_str()))
	{
		for (const auto& [key, value] : choices)
		{
			bool selected = key == currentChoice;

			if (ImGui::Selectable(value.c_str(), selected))
			{
				if (!selected)
				{
					ImGui::SetItemDefaultFocus();
					currentChoice = key;
					ValueChangedEvent.Invoke(currentChoice);
					this->NotifyChange();
				}
			}
		}

		ImGui::EndCombo();
	}

}
