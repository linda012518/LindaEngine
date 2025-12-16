#pragma once

#include "Text.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class TextClickable : public Text
	{
	public:
		TextClickable(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		LindaEngine::EventCallback<> ClickedEvent;
		LindaEngine::EventCallback<> DoubleClickedEvent;
	};
}
