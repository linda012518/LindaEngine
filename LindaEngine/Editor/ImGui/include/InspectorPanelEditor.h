#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class InspectorPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(InspectorPanelEditor)
		void OnImGuiRender();
	};
}
