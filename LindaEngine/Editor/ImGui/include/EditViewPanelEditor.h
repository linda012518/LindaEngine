#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class EditViewPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(EditViewPanelEditor)
		void OnImGuiRender();
	};
}
