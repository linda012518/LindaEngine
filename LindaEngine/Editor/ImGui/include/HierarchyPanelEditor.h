#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class HierarchyPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(HierarchyPanelEditor)
		void OnImGuiRender();
	};
}
