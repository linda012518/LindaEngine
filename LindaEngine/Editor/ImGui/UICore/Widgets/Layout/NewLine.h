#pragma once

#include "Widget.h"

namespace LindaEditor
{
	class NewLine : public Widget
	{
	protected:
		void _Draw_Impl() override;
	};
}
