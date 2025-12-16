#pragma once

#include "DataWidget.h"
#include "WidgetContainer.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class TreeNode : public DataWidget<std::string>, public WidgetContainer
	{
	public:
		TreeNode(const std::string& p_name = "", bool p_arrowClickToOpen = false);

		void Open();

		void Close();

		bool IsOpened() const;

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool selected = false;
		bool leaf = false;

		LindaEngine::EventCallback<> ClickedEvent;
		LindaEngine::EventCallback<> DoubleClickedEvent;
		LindaEngine::EventCallback<> OpenedEvent;
		LindaEngine::EventCallback<> ClosedEvent;

	private:
		bool _arrowClickToOpen = false;
		bool _shouldOpen = false;
		bool _shouldClose = false;
		bool _opened = false;

	};
}
