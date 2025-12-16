#pragma once

#include "Layout/Group.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class MenuList : public Group
	{
	public:
		MenuList(const std::string& p_name, bool p_locked = false);

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool locked;
		LindaEngine::EventCallback<> ClickedEvent;

	private:
		bool _opened;
	};
}
