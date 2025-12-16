#pragma once

#include "Widget.h"
#include "WidgetContainer.h"

namespace LindaEditor
{
	class Group : public Widget, public WidgetContainer
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}
