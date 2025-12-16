#pragma once

struct ImGuiStyle;

namespace LindaEditor
{
	enum class Style
	{
		IM_CLASSIC_STYLE,
		IM_DARK_STYLE,
		IM_LIGHT_STYLE,
		DUNE_DARK,			// https://www.unknowncheats.me/forum/direct3d/189635-imgui-style-settings.html
		DEFAULT_DARK,
		EVEN_DARKER,
		UNITY
	};

	template<Style Style>
	class UIStyle
	{
	public:
		static ImGuiStyle GetStyle();
	};
}

