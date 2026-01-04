#pragma once

#include "InputCodes.h"

namespace LindaEngine
{
	struct Event
	{
		virtual ~Event() = default;
	};

	struct WindowResizeEvent : public Event
	{
		int width;
		int height;
		bool isMinimized;
	};

	struct MouseEvent : public Event
	{
		int x; int y; int wheel;
	};

	struct KeyEvent : public Event
	{
		KeyCode key;
	};

	struct CharEvent : public Event
	{
		char key;
	};
}

