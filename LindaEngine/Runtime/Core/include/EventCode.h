#pragma once

namespace LindaEngine
{
	class EventCode
	{
	#define BasicCount 100000
	public:
		const static int WindowResize = BasicCount + 1;
		const static int LeftMouseButtonDown = BasicCount + 2;
		const static int LeftMouseButtonUp = BasicCount + 3;
		const static int LeftMouseButton = BasicCount + 4;
		const static int RightMouseButtonDown = BasicCount + 5;
		const static int RightMouseButtonUp = BasicCount + 6;
		const static int RightMouseButton = BasicCount + 7;
		const static int MouseMove = BasicCount + 8;
		const static int MouseWheel = BasicCount + 9;
		const static int KeyDown = BasicCount + 10;
		const static int KeyUp = BasicCount + 11;
		const static int Key = BasicCount + 12;
		const static int CharInput = BasicCount + 13;
		const static int LeftMouseButtonClick = BasicCount + 14;
		const static int RightMouseButtonClick = BasicCount + 15;
		const static int KeyClick = BasicCount + 16;
		const static int MouseWheelClick = BasicCount + 17;
		const static int MouseWheelDown = BasicCount + 18;
		const static int MouseWheelUp = BasicCount + 19;
		const static int MouseWheelPressed = BasicCount + 20;
		const static int MouseLeave = BasicCount + 21;
	};
}
