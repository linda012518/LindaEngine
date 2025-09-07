#pragma once

#include <unordered_map>

namespace LindaEngine
{
	class LObject;
	class EventSystem
	{
	public:
		static std::unordered_map<int, std::list<LObject*>> _eventMap;

		static void Bind(int code, LObject* obj);

		static void Dispatch(int code, void* userData);
	};
}
