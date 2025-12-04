#pragma once

#include "AutoPtr.h"
#include "ImGuiPanelEditor.h"

#include <string>
#include <vector>

namespace LindaEditor
{
	class ImGuiContextEditor
	{
	public:
		int Initialize();
		void Finalize();

		void Begin();
		void End();

		static void SetDarkThemeColors();

		void OnImGuiRender();

		void AddPanel();

	private:
		std::vector<Ref<ImGuiPanelEditor>> _panels;
	};
}
