#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class PlayViewPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(PlayViewPanelEditor)
		void OnImGuiRender();
	};
}