#pragma once

#include "AutoPtr.h"
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
		static std::unordered_map<int, std::list<Weak<LindaEngine::IEventHandler>>> _eventMap;

		static void Bind(int code, Weak<LindaEngine::IEventHandler> obj);

		static void Unbind(int code, Weak<LindaEngine::IEventHandler> obj);

		static void Dispatch(Weak<LindaEngine::IEventHandler> sender, int code, LindaEngine::Event& eventData);

		static void Clear();

	};
}
