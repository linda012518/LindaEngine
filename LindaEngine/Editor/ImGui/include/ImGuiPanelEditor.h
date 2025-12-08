#pragma once

#include "ClassDeclare.inl"
#include "ClassImplement.inl"
#include "imgui/imgui.h"

namespace LindaEditor
{
	class ImGuiPanelEditor
	{
	public:
		virtual void OnImGuiRender() = 0;
	};
}
