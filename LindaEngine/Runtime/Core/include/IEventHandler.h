#pragma once

namespace LindaEngine
{
	struct Event;

	class IEventHandler
	{
	public:
		virtual ~IEventHandler() = default;

		virtual void OnEvent(IEventHandler* sender, int eventCode, Event& eventData) = 0;
	};
}

