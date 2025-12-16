#pragma once

#include "Pluginable.h"

#include <string>

namespace LindaEditor
{
	class WidgetContainer;

	class Widget : public Pluginable
	{
	public:
		Widget();

		virtual void Draw();

		void LinkTo(const Widget& p_widget);

		void Destroy();

		bool IsDestroyed() const;

		void SetParent(WidgetContainer* p_parent);

		bool HasParent() const;

		WidgetContainer* GetParent();

	protected:
		virtual void _Draw_Impl() = 0;

	public:
		std::string tooltip;
		bool enabled = true;
		bool disabled = false;
		bool lineBreak = true;

	protected:
		WidgetContainer* _parent;
		std::string _widgetID = "?";
		bool _autoExecutePlugins = true;

	private:
		static uint64_t __WIDGET_ID_INCREMENT;
		bool _destroyed = false;

	};
}
