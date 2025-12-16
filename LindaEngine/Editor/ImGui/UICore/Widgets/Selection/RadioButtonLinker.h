#pragma once

#include "RadioButton.h"

namespace LindaEditor
{
	class RadioButtonLinker : public DataWidget<int>
	{
	public:
		RadioButtonLinker();

		void Link(RadioButton& p_radioButton);

		void Unlink(RadioButton& p_radioButton);

		int GetSelected() const;

	protected:
		void _Draw_Impl() override;

	private:
		void OnRadioButtonClicked(int p_radioID);

	public:
		LindaEngine::EventCallback<int> ValueChangedEvent;

	private:
		int _availableRadioID = 0;
		int _selected = -1;
		std::vector<std::pair<LindaEngine::ListenerID, std::reference_wrapper<RadioButton>>> _radioButtons;

	};
}
