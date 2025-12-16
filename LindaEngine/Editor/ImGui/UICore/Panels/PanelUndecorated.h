#pragma once

#include "PanelTransformable.h"

namespace LindaEditor
{
	class PanelUndecorated : public PanelTransformable
	{
	public:
		void _Draw_Impl() override;

	private:
		int CollectFlags();
	};
}
