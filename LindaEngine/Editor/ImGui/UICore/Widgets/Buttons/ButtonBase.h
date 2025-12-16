#pragma once

#include "Widget.h"

#include "EventCallback.h"

namespace LindaEditor
{
	class ButtonBase : public Widget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		LindaEngine::EventCallback<> ClickedEvent;
	};
}
