#pragma once

#include "Widget.h"

namespace LindaEditor
{
	class Spacing : public Widget
	{
	public:
		Spacing(uint16_t p_spaces = 1);

	protected:
		void _Draw_Impl() override;

	public:
		uint16_t spaces = 1;
	};
}
