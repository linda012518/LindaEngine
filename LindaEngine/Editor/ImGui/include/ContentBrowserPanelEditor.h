#pragma once

#include "ImGuiPanelEditor.h"

namespace LindaEditor
{
	class ContentBrowserPanelEditor : public ImGuiPanelEditor
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(ContentBrowserPanelEditor)
		void OnImGuiRender();
	};
}

