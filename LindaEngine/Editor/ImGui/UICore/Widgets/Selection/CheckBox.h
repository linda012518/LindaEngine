#pragma once

#include "DataWidget.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class CheckBox : public DataWidget<bool>
	{
	public:
		CheckBox(bool p_value = false, const std::string& p_label = "");

	protected:
		void _Draw_Impl() override;

	public:
		bool value;
		std::string label;
		LindaEngine::EventCallback<bool> ValueChangedEvent;
	};
}
