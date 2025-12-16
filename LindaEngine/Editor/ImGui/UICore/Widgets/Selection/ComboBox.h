#pragma once

#include "DataWidget.h"
#include "EventCallback.h"

#include <map>
#include <string>

namespace LindaEditor
{
	class ComboBox : public DataWidget<int>
	{
	public:
		ComboBox(int p_currentChoice = 0);

	protected:
		void _Draw_Impl() override;

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		LindaEngine::EventCallback<int> ValueChangedEvent;
	};
}
