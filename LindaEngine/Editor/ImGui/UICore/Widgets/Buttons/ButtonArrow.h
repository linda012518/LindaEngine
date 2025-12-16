#pragma once

#include "Button.h"
#include "UISettings.h"

namespace LindaEditor
{
	class ButtonArrow : public Button
	{
	public:
		ButtonArrow(Direction p_direction = Direction::NONE);

	protected:
		void _Draw_Impl() override;

	public:
		Direction direction;
	};
}
