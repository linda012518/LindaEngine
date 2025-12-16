#pragma once

namespace LindaEditor
{
	enum class PluginExecutionContext
	{
		WIDGET, PANEL
	};

	class IPlugin
	{
	public:
		virtual void Execute(PluginExecutionContext p_context) = 0;

		void* userData = nullptr;
	};
}
