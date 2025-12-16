#pragma once

#include "IPlugin.h"

#include <vector>

namespace LindaEditor
{
	class Pluginable
	{
	public:
		virtual ~Pluginable()
		{
			RemoveAllPlugins();
		}

		template<typename T, typename... Args>
		T& AddPlugin(Args&& ... p_args)
		{
			static_assert(std::is_base_of<IPlugin, T>::value, "T should derive from IPlugin");

			T* newPlugin = new T(std::forward<Args>(p_args)...);
			_plugins.push_back(newPlugin);
			return *newPlugin;
		}

		template<typename T>
		T* GetPlugin()
		{
			static_assert(std::is_base_of<IPlugin, T>::value, "T should derive from IPlugin");

			for (auto it = _plugins.begin(); it != _plugins.end(); ++it)
			{
				T* result = dynamic_cast<T*>(*it);
				if (result)
					return result;
			}

			return nullptr;
		}

		void ExecutePlugins(PluginExecutionContext p_context)
		{
			for (auto& plugin : _plugins)
				plugin->Execute(p_context);
		}

		void RemoveAllPlugins()
		{
			for (auto& plugin : _plugins)
				delete plugin;

			_plugins.clear();
		}

	private:
		std::vector<IPlugin*> _plugins;

	};
}
