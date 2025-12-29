#pragma once

#include <unordered_map>

namespace LindaEngine
{
	class IEventHandler;
	struct Event;
}

namespace LindaEditor
{
	class EventSystemEditor
	{
	public:
		static std::unordered_map<int, std::list<LindaEngine::IEventHandler*>> _eventMap;

		static void Bind(int code, LindaEngine::IEventHandler* obj);

		static void Unbind(int code, LindaEngine::IEventHandler* obj);

		static void Dispatch(LindaEngine::IEventHandler* sender, int code, LindaEngine::Event& eventData);

		static void Clear();

	};
}
