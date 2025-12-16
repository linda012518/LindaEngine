#pragma once

#include "DataWidget.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class RadioButtonLinker;

	class RadioButton : public DataWidget<bool>
	{
		friend RadioButtonLinker;

	public:
		RadioButton(bool p_selected = false, const std::string& p_label = "");

		void Select();

		bool IsSelected() const;

		bool GetRadioID() const;

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		LindaEngine::EventCallback<int> ClickedEvent;

	private:
		bool _selected = false;
		int _radioID = 0;

	};
}
