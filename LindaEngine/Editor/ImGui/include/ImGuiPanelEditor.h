#pragma once

#include "ClassDeclare.inl"
#include "ClassImplement.inl"

namespace LindaEditor
{
	class ImGuiPanelEditor
	{
	public:
		virtual void OnImGuiRender() = 0;
	};
}
