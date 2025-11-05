#pragma once

namespace LindaEditor
{
	class ImGuiContext
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
