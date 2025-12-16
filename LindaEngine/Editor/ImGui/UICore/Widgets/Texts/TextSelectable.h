#pragma once

#include "Text.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class TextSelectable : public Text
	{
	public:
		TextSelectable(const std::string& p_content = "", bool p_selected = false, bool p_disabled = false);

	protected:
		virtual void _Draw_Impl() override;

	public:
		bool selected;
		bool disabled;

		LindaEngine::EventCallback<bool> ClickedEvent;
		LindaEngine::EventCallback<> SelectedEvent;
		LindaEngine::EventCallback<> UnselectedEvent;
	};
}
