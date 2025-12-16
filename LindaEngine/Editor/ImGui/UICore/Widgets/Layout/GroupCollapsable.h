#pragma once

#include "Group.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class GroupCollapsable : public Group
	{
	public:
		GroupCollapsable(const std::string& p_name = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		bool collapsed = false;
		LindaEngine::EventCallback<> CloseEvent;
		LindaEngine::EventCallback<> OpenEvent;
	};
}
