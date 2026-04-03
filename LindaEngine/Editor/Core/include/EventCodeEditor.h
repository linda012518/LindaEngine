#pragma once

namespace LindaEditor
{
	class EventCodeEditor
	{
#define BasicCountEditor 1000000
	public:
		const static int SwitchSelectEntity = BasicCountEditor + 1;
		const static int PickEntityID = BasicCountEditor + 2;
		const static int SwitchInspectorObject = BasicCountEditor + 3;
	};
}
