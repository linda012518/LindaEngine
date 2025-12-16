#pragma once

namespace LindaEditor
{
	enum class HorizontalAlignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum class VerticalAlignment
	{
		TOP,
		MIDDLE,
		BOTTOM
	};

	enum class Direction
	{
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		UP = 3,
		DOWN = 4,
	};

	struct PanelWindowSettings
	{
		bool closable = false;
		bool resizable = true;
		bool movable = true;
		bool dockable = false;
		bool scrollable = true;
		bool hideBackground = false;
		bool forceHorizontalScrollbar = false;
		bool forceVerticalScrollbar = false;
		bool allowHorizontalScrollbar = false;
		bool bringToFrontOnFocus = true;
		bool collapsable = false;
		bool allowInputs = true;
		bool titleBar = true;
		bool autoSize = false;
	};
}