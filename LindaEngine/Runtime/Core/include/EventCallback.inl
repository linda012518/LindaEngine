#pragma once

#include "EventCallback.h"

namespace LindaEngine
{
	template<class... ArgTypes>
	ListenerID EventCallback<ArgTypes...>::AddListener(Callback p_callback)
	{
		ListenerID listenerID = _availableListenerID++;
		_callbacks.emplace(listenerID, p_callback);
		return listenerID;
	}

	template<class... ArgTypes>
	ListenerID EventCallback<ArgTypes...>::operator+=(Callback p_callback)
	{
		return AddListener(p_callback);
	}

	template<class... ArgTypes>
	bool EventCallback<ArgTypes...>::RemoveListener(ListenerID p_listenerID)
	{
		return _callbacks.erase(p_listenerID) != 0;
	}

	template<class... ArgTypes>
	bool EventCallback<ArgTypes...>::operator-=(ListenerID p_listenerID)
	{
		return RemoveListener(p_listenerID);
	}

	template<class... ArgTypes>
	void EventCallback<ArgTypes...>::RemoveAllListeners()
	{
		_callbacks.clear();
	}

	template<class... ArgTypes>
	uint64_t EventCallback<ArgTypes...>::GetListenerCount()
	{
		return _callbacks.size();
	}

	template<class... ArgTypes>
	void EventCallback<ArgTypes...>::Invoke(ArgTypes... p_args)
	{
		for (auto const& [key, value] : _callbacks)
			value(p_args...);
	}
}