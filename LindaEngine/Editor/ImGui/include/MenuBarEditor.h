#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class MenuBarEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(MenuBarEditor)
		void OnImGuiRender();
	};
}

