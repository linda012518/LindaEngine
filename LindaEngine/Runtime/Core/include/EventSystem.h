#pragma once

#include <unordered_map>

namespace LindaEngine
{
	class IEventHandler;
	struct Event;

	class EventSystem
	{
	public:
		static std::unordered_map<int, std::list<IEventHandler*>> _eventMap;

		static void Bind(int code, IEventHandler* obj);

		static void Unbind(int code, IEventHandler* obj);

		static void Dispatch(IEventHandler* sender, int code, Event& eventData);
	};
}
