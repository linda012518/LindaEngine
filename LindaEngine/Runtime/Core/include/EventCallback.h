#pragma once

#include <functional>

namespace LindaEngine
{
	using ListenerID = uint64_t;

	template<class... ArgTypes>
	class EventCallback
	{
	public:
		using Callback = std::function<void(ArgTypes...)>;

		ListenerID AddListener(Callback p_callback);

		ListenerID operator+=(Callback p_callback);

		bool RemoveListener(ListenerID p_listenerID);

		bool operator-=(ListenerID p_listenerID);

		void RemoveAllListeners();

		uint64_t GetListenerCount();

		void Invoke(ArgTypes... p_args);

	private:
		std::unordered_map<ListenerID, Callback>	_callbacks;
		ListenerID									_availableListenerID = 0;
	};

}

#include "EventCallback.inl"
