#pragma once

namespace LindaEditor
{
	class ImGuiContextEditor
	{
	public:
		int Initialize();
		void Finalize();

		void Begin();
		void End();
		void SetDarkThemeColors();

		void OnImGuiRender();
	};
}
