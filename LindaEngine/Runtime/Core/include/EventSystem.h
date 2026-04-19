#pragma once

#include "AutoPtr.h"
#include <unordered_map>

namespace LindaEngine
{
	class IEventHandler;
	struct Event;

	class EventSystem
	{
	public:
		static std::unordered_map<int, std::list<Weak<IEventHandler>>> _eventMap;

		static void Bind(int code, Weak<IEventHandler> obj);

		static void Unbind(int code, Weak<IEventHandler> obj);

		static void Dispatch(Weak<IEventHandler> sender, int code, Event& eventData);

		static void Clear();
	};
}
