#pragma once

#include <unordered_map>

namespace LindaEngine
{
	class LObject;
	struct Event;

	class EventSystem
	{
	public:
		static std::unordered_map<int, std::list<LObject*>> _eventMap;

		static void Bind(int code, LObject* obj);

		static void Unbind(int code, LObject* obj);

		static void Dispatch(LObject* sender, int code, Event& eventData);
	};
}
