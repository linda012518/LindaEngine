#pragma once

#include "DataWidget.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class MenuItem : public DataWidget<bool>
	{
	public:
		MenuItem(const std::string& p_name, const std::string& p_shortcut = "", bool p_checkable = false, bool p_checked = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string name;
		std::string shortcut;
		bool checkable;
		bool checked;
		LindaEngine::EventCallback<> ClickedEvent;
		LindaEngine::EventCallback<bool> ValueChangedEvent;

	private:
		bool _selected;

	};
}
