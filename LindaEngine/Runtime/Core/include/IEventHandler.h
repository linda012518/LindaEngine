#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct Event;

	class IEventHandler
	{
	public:
		virtual ~IEventHandler() = default;

		virtual void OnEvent(Weak<IEventHandler> sender, int eventCode, Event& eventData) = 0;
	};
}

