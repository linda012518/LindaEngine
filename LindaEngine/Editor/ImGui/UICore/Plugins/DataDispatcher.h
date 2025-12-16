#pragma once

#include "IPlugin.h"

#include <functional>

namespace LindaEditor
{

#define TRY_GATHER(type, output)	if (auto plugin = GetPlugin<DataDispatcher<type>>(); plugin) output = plugin->Gather();
#define TRY_PROVIDE(type, output)	if (auto plugin = GetPlugin<DataDispatcher<type>>(); plugin) plugin->Provide(output);
#define TRY_NOTIFY_CHANGE(type)		if (auto plugin = GetPlugin<DataDispatcher<type>>(); plugin) plugin->NotifyChange();

	template <typename T>
	class DataDispatcher : public IPlugin
	{
	public:
		void RegisterReference(T& p_reference)
		{
			_dataPointer = &p_reference;
		}

		void RegisterProvider(std::function<void(T)> p_provider)
		{
			_provider = p_provider;
		}

		void RegisterGatherer(std::function<T(void)> p_gatherer)
		{
			_gatherer = p_gatherer;
		}

		void Provide(T p_data)
		{
			if (_valueChanged)
			{
				if (_dataPointer)
					*_dataPointer = p_data;
				else
					_provider(p_data);

				_valueChanged = false;
			}
		}

		void NotifyChange()
		{
			_valueChanged = true;
		}

		T Gather()
		{
			return _dataPointer ? *_dataPointer : _gatherer();
		}

		virtual void Execute(PluginExecutionContext p_context) override {}

	private:
		bool _valueChanged = false;
		T* _dataPointer = nullptr;
		std::function<void(T)> _provider;
		std::function<T(void)> _gatherer;

	};
}
