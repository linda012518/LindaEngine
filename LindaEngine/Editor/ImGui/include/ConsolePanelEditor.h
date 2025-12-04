#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class ConsolePanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(ConsolePanelEditor)
		void OnImGuiRender();
	};
}
