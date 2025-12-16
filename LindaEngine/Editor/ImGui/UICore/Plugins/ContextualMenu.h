#pragma once

#include "IPlugin.h"
#include "WidgetContainer.h"

namespace LindaEditor
{
	class ContextualMenu : public IPlugin, public WidgetContainer
	{
	public:
		void Execute(PluginExecutionContext p_context);

		void Close();
	};
}
