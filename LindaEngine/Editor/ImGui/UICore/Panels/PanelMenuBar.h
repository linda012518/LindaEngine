#pragma once

#include "Panel.h"

namespace LindaEditor
{
	class PanelMenuBar : public Panel
	{
	protected:
		void _Draw_Impl() override;
	};
}
